<?php
include_once 'config.php';

if (!$_SESSION['admin'] && !$_SESSION['user'])
{
    header("Location: aut.php");
    exit();
}

if ($_SESSION['admin'])
{
    header("Location: admin/index.php");
    exit();
}

$row_u = m_q("SELECT * FROM users WHERE id='".$_SESSION['id']."'", 2);
if ($row_u['last_login'] < time()-60)//Обновляем онлайн не чаще раз в минуту
    m_q("UPDATE users SET last_login='".time()."' WHERE id='".$_SESSION['id']."'");
$row_q = m_q("SELECT * FROM quizones WHERE id='".$row_u['quizone']."'", 2);

//Прием ответа на вопрос
if ($_POST['set_answer'])
{
    if (check_csrf($_POST['csrf']))
    {
        $id = (int)$_POST['set_answer'];
        $answer = (int)$_POST['answer'];
        $row = m_q("SELECT * FROM user_questions WHERE id='$id'", 2);//Ищем вопрос
        if ($row['id'] && $row['status'] == 0 && $row['user'] == $_SESSION['id'])
        {
            //Зачисляем
            $summ = 0;
            if ($row['id_cat'] == $answer)//Ответ верный
            {
                if ($row['help'] || $row['help_user'])//Была подсказка
                    $summ -= 1;
                $summ += 2;
                m_q("START TRANSACTION");
                m_q("UPDATE user_questions SET time2='".time()."', answer='$answer', result='$summ', status='1' WHERE id='".$row['id']."'");
                m_q("UPDATE users SET balance=balance+'$summ' WHERE id='".$row['user']."'");
                m_q("COMMIT");
                $row_u = m_q("SELECT * FROM users WHERE id='".$row['user']."'", 2);
                echo json_encode(array('result'=>'Ok', 'class'=>'btn-success', 'balance'=>$row_u['balance'].' балов'));
            }
            else
            {
                //Ответ неверный
                $summ -= 1;
                if ($row_u['balance'] <= 0)//Минусового баланса нет
                    $summ = 0;
                m_q("START TRANSACTION");
                m_q("UPDATE user_questions SET time2='".time()."', answer='$answer', result='$summ', status='2' WHERE id='".$row['id']."'");
                m_q("UPDATE users SET balance=balance+'$summ' WHERE id='".$row['user']."'");
                m_q("COMMIT");
                $row_u = m_q("SELECT * FROM users WHERE id='".$row['user']."'", 2);
                echo json_encode(array('result'=>'Ok', 'class'=>'btn-danger', 'balance'=>$row_u['balance'].' балов'));
            }
        }
        else
            echo json_encode(array('result'=>'Error', 'text'=>'Вопрос не найден. Обновите страницу.'));
    }
    else
        echo json_encode(array('result'=>'Error', 'text'=>'Форма устарела, обновите страницу.'));
    exit();
}

//Метка получения помощи системы
if ($_POST['get_help'])
{
    if (check_csrf($_POST['csrf']))
    {
        $id = (int)$_POST['get_help'];
        $row = m_q("SELECT * FROM user_questions WHERE id='$id'", 2);//Ищем вопрос
        if ($row['id'] && $row['status'] == 0 && $row['user'] == $_SESSION['id'])
        {
            m_q("UPDATE user_questions SET help='1' WHERE id='".$row['id']."'");
            echo json_encode(array('result'=>'Ok'));
        }
        else
            echo json_encode(array('result'=>'Error', 'text'=>'Вопрос не найден. Обновите страницу.'));
    }
    else
        echo json_encode(array('result'=>'Error', 'text'=>'Форма устарела, обновите страницу.'));
    exit();
}

//Метка получения помощи пользователей
if ($_POST['get_help_user'])
{
    if (check_csrf($_POST['csrf']))
    {
        $id = (int)$_POST['get_help_user'];
        $row = m_q("SELECT * FROM user_questions WHERE id='$id'", 2);//Ищем вопрос
        if ($row['id'] && $row['status'] == 0 && $row['user'] == $_SESSION['id'])
        {
            m_q("UPDATE user_questions SET help_user='1' WHERE id='".$row['id']."'");
            echo json_encode(array('result'=>'Ok'));
        }
        else
            echo json_encode(array('result'=>'Error', 'text'=>'Вопрос не найден. Обновите страницу.'));
    }
    else
        echo json_encode(array('result'=>'Error', 'text'=>'Форма устарела, обновите страницу.'));
    exit();
}

//Выдача вопроса из нужной категории юзера (и его квизона)
if ($_POST['get_question'])
{
    if (check_csrf($_POST['csrf']))
    {
        if ($_SESSION['id'])//Есть авторизация
        {
            $row_u = m_q("SELECT * FROM users WHERE id='".$_SESSION['id']."'", 2);
            if ($row_u['id'])
            {
                //Смотрим категорию в квизоне
                $row = m_q("SELECT * FROM quizones WHERE id='".$row_u['quizone']."' AND status='1'", 2);
                if ($row['id'])
                {
                    //Получаем категорию
                    $row2 = m_q("SELECT * FROM categories WHERE id='".$row['category']."' AND status='1'", 2);
                    if ($row2['id'])
                    {
                        //Смотрим есть ли выделенный вопрос
                        $row3 = m_q("SELECT * FROM user_questions WHERE user='".$row_u['id']."' AND category_id='".$row2['id']."' AND status='0'", 2);
                        if ($row3['id'])
                        {
                            $uq_id = $row3['id'];
                            $row4 = m_q("SELECT * FROM questions WHERE id='".$row3['question_id']."'", 2);
                        }
                        else
                        {
                            //Берем рандомный вопрос из категории, на который еще не отвечали правильно
                            $res4 = m_q("SELECT * FROM questions WHERE category='".$row2['id']."' AND status='1' ORDER BY RAND() LIMIT 0, 250", 1);
                            while ($row4 = m_f_a($res4))
                            {
                                $row5 = m_q("SELECT * FROM user_questions WHERE user='".$row_u['id']."' AND question_id='".$row4['id']."' AND status='1'", 2);
                                if (!$row5['id'])//Не нашли с правильным ответом
                                {
                                    //Фиксируем в БД
                                    m_q("INSERT INTO user_questions VALUES ('', '".time()."', '', '".$row_u['id']."', '".$row_u['quizone']."', '".$row2['id']."', '".$row4['id']."', '".$row4['id_cat']."', '0', '0', '0', '0', '0')");
                                    $uq_id = m_i_i();
                                    break;//Переходим к выдаче
                                }
                                else
                                    $row4 = array();//Пропускаем
                            }
                        }
                        if ($row4['id'])//Есть вопрос
                        {
                            //Вопрос
                            $text = $row4['text'];
                            if ($row4['type'] == 2)//Картинка
                                $text = '<a href="'.$g_url.$row4['img'].'" target="_blank"><img src="'.$g_url.$row4['img'].'" style="max-width: 1000px; max-height: 300px;"></a>';
                            if ($row4['type'] == 3)//Аудио
                                $text = '<audio src="'.$g_url.$row4['audio'].'" controls></audio>';
                            //Кнопки ответов
                            $buttons = '';
                            $res5 = m_q("SELECT id_cat FROM questions WHERE category='".$row4['category']."' AND status='1'", 1);
                            while ($row5 = m_f_a($res5))
                            {
                                $buttons .= '
                                    <button type="button" class="btn btn-info btn-lg" style="margin-bottom: 5px;" onclick="set_answer('.$row3['id'].', '.$row5[0].', this);">'.$row5[0].'</button>
                                ';
                            }
                            //Подсказка
                            if ($row3['help'])//Уже активировали
                            {
                                $help_block = $row4['help_text'];
                                if ($row4['help_type'] == 2)//Картинка
                                    $help_block = '<a href="'.$g_url.$row4['help_img'].'" target="_blank"><img src="'.$g_url.$row4['help_img'].'" style="max-width: 1000px; max-height: 300px;"></a>';
                                if ($row4['help_type'] == 3)//Аудио
                                    $help_block = '<audio src="'.$g_url.$row4['help_audio'].'" controls></audio>';
                            }
                            else
                            {
                                //Кнопка активации
                                $help_block = '
                                    <button type="button" class="btn btn-default" onclick="get_help('.$row3['id'].');">Получить подсказку</button>
                                ';
                            }
                            //Помощь пользователей
                            if ($row3['help_user'])//Уже активировали
                            {
                                $res4 = m_q("SELECT h.*, u.login FROM user_helps h LEFT JOIN users u ON u.id=h.user WHERE h.user_question_id='".$row3['id']."'", 1);
                                while ($row4 = m_f_a($res4))
                                    $help_user_block .= 'Пользователь "'.$row4['login'].'": '.$row4['val'].'<br>';
                                if (!$help_user_block)
                                {
                                    //Чекаем может ли кто-то помочь
                                    $row4 = m_q("SELECT COUNT(q.*) FROM user_questions q LEFT JOIN users u ON u.id=q.user WHERE q.question_id='".$row3['question_id']."' AND q.status='1' AND u.last_login>'".(time()-2*60)."' AND u.quizone='".$row3['quizone']."'", 2);
                                    if ($row4[0])//Есть кто-то
                                        $help_user_block = '... Asking others...';
                                    else
                                        $help_user_block = 'No one answered this question yet';
                                }
                            }
                            else
                            {
                                //Выдаем кнопку
                                $help_user_block = '
                                    <button type="button" class="btn btn-default" onclick="get_help_user('.$row3['id'].')">Запросить помощь</button>
                                ';
                            }
                            
                            //Формирование итогового блока (обновляется при изменении вопроса)
                            $question .= '
                                <div class="row">
                                    <div class="col-md-12" style="font-size: 175%; font-weight: bold;">
                                        <p>'.$text.'</p>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        '.$buttons.'
                                    </div>
                                </div>
                                <hr>
                            ';
                            //Блок помощи
                            $help .= '
                                <div class="row">
                                    <div class="col-md-12">
                                    <p><b><big>Подсказка:</big></b></p>
                                        '.$help_block.'
                                    </div>
                                </div>
                                <hr>
                            ';
                            //Блок с помощью юзера (его обновляем всегда)
                            $help_user .= '
                                <div class="row">
                                    <div class="col-md-12">
                                        <p><b><big>Помощь участников:</big></b></p>
                                        '.$help_user_block.'
                                    </div>
                                </div>
                            ';
                        }
                        else
                        {
                            $question .= '
                                <b>Все вопросы категории "'.$row2['name'].'" закончились.</b>
                            ';
                        }
                    }
                }
            }
        }
        if (!$question)
            $question = '<b>Нет подходящих заданий.</b>';
        echo json_encode(array('result'=>'Ok', 'question_id'=>(int)$row3['id'], 'html_question'=>$question, 'html_help'=>$help, 'html_help_user'=>$help_user, 'title'=>$row2['name']));
    }
    else
        echo json_encode(array('result'=>'Error', 'text'=>'Форма устарела, обновите страницу.'));
    exit();
}

//Прием запроса на помощь
if ($_POST['set_helping_user'])
{
    if (check_csrf($_POST['csrf']))
    {
        $id = (int)$_POST['set_helping_user'];
        $answer = (int)$_POST['answer'];
        $row = m_q("SELECT * FROM user_questions WHERE question_id='$id' AND user='".$_SESSION['id']."' AND status='1'", 2);//Ищем ответ у юзера
        if ($row['id'])//Нашли
        {
            //Ищем запрос помощи по этому вопросу
            $row2 = m_q("SELECT * FROM user_questions WHERE question_id='".$id."' AND help_user='1' AND user != '".$row['user']."' AND status='0'", 2);
            if ($row2['id'])
            {
                //Проверяем наличие ответа ранее
                $row3 = m_q("SELECT * FROM user_helps WHERE user='".$row['user']."' AND user_question_id='".$row2['id']."'", 2);
                if (!$row3['id'])
                {
                    $summ = 1;//Приз за помощь
                    m_q("START TRANSACTION");
                    m_q("INSERT INTO user_helps VALUES ('', '".time()."', '".$row['user']."', '".$row2['quizone']."', '".$row2['category_id']."', '".$row2['question_id']."', '".$row2['id']."', '$answer', '$summ')");
                    m_q("UPDATE users SET balance=balance+'$summ' WHERE id='".$row['user']."'");
                    m_q("COMMIT");
                    $row_u = m_q("SELECT * FROM users WHERE id='".$row['user']."'", 2);
                    echo json_encode(array('result'=>'Ok', 'class'=>'btn-success', 'balance'=>$row_u['balance'].' балов'));
                }
                else
                    echo json_encode(array('result'=>'Error', 'text'=>'Запрос помощи более не актуален. Обновите страницу.'));
            }
            else
                echo json_encode(array('result'=>'Error', 'text'=>'Запрос помощи более не актуален. Обновите страницу.'));
        }
        else
            echo json_encode(array('result'=>'Error', 'text'=>'Запрос не корректный. Обновите страницу.'));
    }
    else
        echo json_encode(array('result'=>'Error', 'text'=>'Форма устарела, обновите страницу.'));
    exit();
}

//Запрос на подсказки пользователям
if ($_POST['get_helping_user'])
{
    if (check_csrf($_POST['csrf']))
    {
        if ($_SESSION['id'])//Есть авторизация
        {
            $row_u = m_q("SELECT * FROM users WHERE id='".$_SESSION['id']."'", 2);
            if ($row_u['id'])
            {
                //Смотрим категорию в квизоне
                $row = m_q("SELECT * FROM quizones WHERE id='".$row_u['quizone']."' AND status='1'", 2);
                if ($row['id'])
                {
                    //Получаем категорию
                    $row2 = m_q("SELECT * FROM categories WHERE id='".$row['category']."' AND status='1'", 2);
                    if ($row2['id'])
                    {
                        //Ищем запросы на помощь
                        $res3 = m_q("SELECT q.*, u.login FROM user_questions q LEFT JOIN users u ON u.id=q.user WHERE q.help_user='1' AND q.status='0' LIMIT 0, 250", 1);
                        while ($row3 = m_f_a($res3))
                        {
                            //Ищем верный ответ у себя
                            $row4 = m_q("SELECT * FROM user_questions WHERE user='".$row_u['id']."' AND question_id='".$row3['question_id']."' AND status='1'", 2);
                            if ($row4['id'])
                            {
                                //Ищем наличие помощи ранее
                                $row5 = m_q("SELECT * FROM user_helps WHERE user='".$row_u['id']."' AND user_question_id='".$row3['id']."'", 2);
                                if (!$row5['id'])
                                    break;
                                else
                                    $row3 = array();
                            }
                            else
                                $row3 = array();
                        }
                        if ($row3['id'])//Нашли нужный вопрос с ответом верным
                        {
                            $row5 = m_q("SELECT * FROM questions WHERE id='".$row3['question_id']."'", 2);
                            //Вопрос
                            $text = $row5['text'];
                            if ($row5['type'] == 2)//Картинка
                                $text = '<a href="'.$g_url.$row5['img'].'" target="_blank"><img src="'.$g_url.$row5['img'].'" style="max-width: 1000px; max-height: 300px;"></a>';
                            if ($row5['type'] == 3)//Аудио
                                $text = '<audio src="'.$g_url.$row5['audio'].'" controls></audio>';
                            $html_helping_user = '
                                <div class="row">
                                    <div class="col-md-12">
                                        <hr>
                                        <p><b><big>Помогите участнику "'.$row3['login'].'":</big></b></p>
                                        '.$text.'
                                        <input type="text" id="input_helping_user" class="form-control col-md-6 offset-md-3" placeholder="Ваш ответ"><br>
                                        <button type="button" class="btn btn-info" onclick="set_helping_user('.$row5['id'].', $(\'#input_helping_user\').val(), this);">Отправить</button>
                                    </div>
                                </div>
                            ';
                            echo json_encode(array('result'=>'Ok', 'helping_user_id'=>(int)$row3['question_id'], 'html_helping_user'=>$html_helping_user));
                            exit();
                        }
                    }
                }
            }
        }
    }
    echo json_encode(array('result'=>'Ok', 'helping_user_id'=>0, 'html_helping_user'=>''));
    exit();
}

content('
    <div class="row">
        <div class="col-md-12" style="text-align: center;" id="html_question">
            
        </div>
    </div>
    <div class="row">
        <div class="col-md-12" style="text-align: center;" id="html_help">
            
        </div>
    </div>
    <div class="row">
        <div class="col-md-12" style="text-align: center;" id="html_help_user">
            
        </div>
    </div>
    <div class="row">
        <div class="col-md-12" style="text-align: center;" id="html_helping_user">
            
        </div>
    </div>
');

$out['add_script'] .= '
    <script>
        block_click = new Array();
        question_id = -1;
        function get_question(update_all = 0)
        {
            $.post("", {get_question: 1, csrf: "'.get_csrf().'"}, function (data) {
                res = $.parseJSON(data);
                if (res.result == "Ok")
                {
                    if (res.question_id != question_id)
                    {
                        question_id = res.question_id
                        $("#html_question").html(res.html_question);
                        $("#html_help").html(res.html_help);
                        $("#title").html(res.title);
                    }
                    if (update_all)
                        $("#html_help").html(res.html_help);
                    $("#html_help_user").html(res.html_help_user);
                }
                else
                    alert(res.text);
            });
        }
        get_question();
        setInterval(get_question, 5000);


        function set_answer(id, answer, but)
        {
            if (!block_click[id])
            {
                block_click[id] = 1;
                $.post("", {set_answer: id, answer, csrf: "'.get_csrf().'"}, function (data) {
                    res = $.parseJSON(data);
                    if (res.result == "Ok")
                    {
                        $(but).removeClass("btn-info");
                        $(but).addClass(res.class);
                        $("#balance").html(res.balance);
                        setTimeout(get_question, 2000);
                    }
                    else
                        alert(res.text);
                });
            }
        }

        function get_help(id)
        {
            $.post("", {get_help: id, csrf: "'.get_csrf().'"}, function (data) {
                res = $.parseJSON(data);
                if (res.result == "Ok")
                {
                    get_question(1);
                }
                else
                    alert(res.text);
            });
        }

        function get_help_user(id)
        {
            $.post("", {get_help_user: id, csrf: "'.get_csrf().'"}, function (data) {
                res = $.parseJSON(data);
                if (res.result == "Ok")
                {
                    get_question();
                }
                else
                    alert(res.text);
            });
        }

        helping_user_id = -1;
        function get_helping_user()
        {
            $.post("", {get_helping_user: 1, csrf: "'.get_csrf().'"}, function (data) {
                res = $.parseJSON(data);
                if (res.result == "Ok")
                {
                    if (res.helping_user_id != helping_user_id)
                    {
                        helping_user_id = res.helping_user_id
                        $("#html_helping_user").html(res.html_helping_user);
                    }
                }
                else
                    alert(res.text);
            });
        }
        get_helping_user();
        setInterval(get_helping_user, 5000);

        function set_helping_user(id, answer, but)
        {
            $.post("", {set_helping_user: id, answer, csrf: "'.get_csrf().'"}, function (data) {
                res = $.parseJSON(data);
                if (res.result == "Ok")
                {
                    $(but).removeClass("btn-info");
                    $(but).addClass(res.class);
                    $("#balance").html(res.balance);
                    setTimeout(get_helping_user, 2000);
                }
                else
                    alert(res.text);
            });
        }

    </script>
';



$out['title'] = "Quizone - ".$row_q['name'];
include 'tmpl.php';

?>