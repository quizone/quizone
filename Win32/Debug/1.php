
<?php
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Contact Form HTML/CSS Template - reusable form</title>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
        <link href="https://fonts.googleapis.com/css?family=Montserrat" rel="stylesheet">
        <link rel="stylesheet" href="form.css" >
        <script src="form.js"></script>
    
    </head>
    <body >
       

<?php

//echo "Hi";
//session_start();
//$_SESSION["item"]="MyItem";
//if ($_SESSION == "")
//    echo "No Session";
//else
//    print_r($_SESSION);

$LastQuest = "0";
$AnswerId = "0";

if (empty($_POST["QuestionId"])) {
    $LastQuest = "0";
  } else {
    $LastQuest = test_input($_POST["QuestionId"]);
  }

if (empty($_POST["submit"])) {
    $AnswerId = "0";
  } else {
    $AnswerId = test_input($_POST["submit"]);
  }

if (empty($_POST["Points"])) {
    $Points = "0";
  } else {
    $Points = test_input($_POST["Points"]);
  }

function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
}

#echo $LastQuest;
#echo $AnswerId;

include 'conn.php';

$conn = OpenCon();

//echo "Connected Successfully";

// Find the current Quiz. Save it to QuizId
$QuizId = 0;
$sql = "SELECT QuizID FROM current";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $QuizId = $row["QuizID"];
     }
}

// Find the table name and number of questions. Save questions as QuestNum and table name as TableName
$QuestNum =0;
$TableName = "";
$sql = "SELECT Num FROM Quizes where id = $QuizId ";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $QuestNum = $row["Num"];
    }
}
$sql = "SELECT TableName FROM Quizes where id = $QuizId ";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $TableName = $row["TableName"];
    }
}

// see result of the last question

if ($AnswerId!="0")
    {
     if ($AnswerId == $LastQuest)
            {
                $Points = $Points + 1; 
            }
    else
            {
                $Points = $Points - 1; 
                if ($Points<0) $Points = 0;
            }
        $sql = "INSERT INTO `foralexon`.`actions` (`Zone`, `Quiz`, `Question`,`Answer`) VALUES ('1', '$QuizId', '$LastQuest' , '$AnswerId' );";     
        $result = $conn->query($sql); 
    }
    
// Find the title and save it to Title
 
$sql = "SELECT Name FROM Quizes where id = $QuizId ";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $Title = $row["Name"];
   //     echo $Title. "<br>";; 
    }
}

// randomize, and give a question    

 $val = mt_rand(1,$QuestNum);
 $Question = "";
 $Hint = "";
 //echo $val. "<br>";
$sql = "SELECT Question,Hint FROM $TableName WHERE id = $val";

$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $Question = $row["Question"];
        $Hint = $row["Hint"];
 //       echo $Question; 
    }
}
    else
   //     echo "no records";
?> 

<script>
function showHint(str) {
    document.getElementById("TheHint").innerHTML = "<?php echo $Hint; ?>";
    }
function showProcessing(str) {
    document.getElementById("TheHint").innerHTML = "<?php echo $Question." -- Processing Your answer"; ?>";
    var Theid = '';
         <?php 
         for ($i = 1; $i<=$QuestNum; $i++)
        {
        ?>    
        document.getElementById("<?php echo "submit"."$i"; ?>").innerHTML = "--";
        <?php
        } ?>
    }

    
</script>

 <div class="container">
            <div id="form-main">
                <div id="form-div">
                    <form class="montform" method="post" enctype = "multipart/form-data" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">  
                        <div style="color:white" align="center"><p class="name">
                            <?php  echo "<h1>$Title</h1>"; ?>
                        </p>
                        <?php
                        if ($AnswerId!="0")
                            {
                            if ($AnswerId == $LastQuest)
                                {
                                ?><div style="color:lightgreen" align="right"><p class="name"><?php echo "Points: ".$Points; ?>            
                                </p>
                                </div>
                                <?php
                                }
                             else
                             {
                                ?><div style="color:red" align="right"><p class="name"><?php echo "Points: ".$Points; ?>            
                                </p>
                                </div>
                                <?php
                                }    
                            }
                            ?>
                    </div>
                         <p class="text" id="Question"><?php  echo "<h1>$Question</h1>"; ?></p>
                            <p class="text" id="TheHint" value="a" Onclick="showHint()">Press for Hint</p>
                         </p>
                         <p>&nbsp;</p>
                          <p class="text">
                        <div class="submit">
                            <?php
                            for ($i = 1; $i<=$QuestNum; $i++)
                            {
                            ?>
                            <button type='submit' id = <?php echo "submit"."$i"; ?> name='submit' Onclick="showProcessing()" value = '<?php echo $i;?>' class="button-blue"><?php echo $i; ?></button>
                            
                            <?php } ?>
                            
                            <input type='hidden' name='Points' value='<?php echo $Points;?>'/> 
                            <input type='hidden' name='QuestionId' value='<?php echo $val;?>'/> 
                            <div class="ease"></div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
<?php
CloseCon($conn);

?>  
        
    </body>
</html>