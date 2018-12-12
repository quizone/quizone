#ifndef GEOMETS
#define GEOMETS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>



#ifndef PI
#define PI   3.14159265358979
#endif

#ifndef PI2
#define PI2  6.28318530717959
#endif

#ifndef PI12
#define PI12  1.57079632679489
#endif

#ifndef DTOR
#define DTOR 0.01745329252
#endif

#ifndef RTOD
#define RTOD 57.295779513082
#endif

#ifndef VECT_SET
 typedef struct vector {
 double x;
 double y;
 double z;
 } VECT;


typedef struct
{
	VECT P1,P2;
} D_VECT;  //２重ベクトル型


typedef struct dvector {
double x1;
double y1;
double z1;
double x2;
double y2;
double z2;
} DVECT;


typedef struct
{
   VECT V1,V2;
} VECTS;


#define VECT_SET
#endif



#ifndef XY_SET
 typedef struct xy {
 double x;
 double y;
 } XY;
 #define XY_SET
#endif


#ifndef _VIEW
 #define _VIEW

typedef struct  /* 視点の設定構造体*/
{
double x;    /*視点位置x*/
double y;    /*視点位置y*/
double z;    /*視点位置z*/
double q1;   /*視角*/
double q2;   /*視角*/
double q3;   /*視角*/
double f;    /*焦点距離*/
} VIEW;

double _geo_getvectdistangle(VECT VP1,VECT VP2); //直交座標２点の距離角(全角)の取得


int _geo_SepaHMS(double angle,int *hour,int *min,double *sec);


void _geo_rotatexy( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,      //回転角正弦
double a_cos);     //回転角余弦

void _geo_rotatexz( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,     //回転角正弦
double a_cos);     //回転角余弦

void _geo_rotateyz( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,     //回転角正弦
double a_cos);     //回転角余弦


void _geo_rotate_vector( /*３次元ベクトルを回転する関数*/

VECT   *,  /*被回転ベクトルの構造体*/
VECT   *,  /*回転後ベクトルの構造体*/
double  ,     /*第一回転角*/
double  ,     /*第２回転角*/
double );     /*第３回転角*/


int _geo_sphere_line_sect( /*球と直線の交点座標の取得関数*/
/*返値  実数解:1 / 虚数解:-1*/
DVECT *,   /*直線の通過座標（２点）*/
DVECT *,   /*交点座標              */
VECT   ,   /*球の中心座標          */
double);   /*球の半径              */

int _geo_reflact( /*屈折計算関数*/
double ,    /*屈折率*/
double ,    /*入射角*/
double *);  /*出射角*/

XY Xy(double x,double y);


int _geo_circlevector_sect (/*円とベクトルの交差判定関数*/
XY      ,     /*ベクトルの始点座標  */
double  ,     /*ベクトルの角度      */
double  ,     /*レンズの曲率半径*/
XY      ,     /*半径の中心座標  */
XY     *,     /*交点座標１=Ｘ座標の手前方*/
XY     *,     /*交点座標２=Ｘ座標の向こう方*/
double *,     /*交点座標１の中心角*/
double *);    /*交点座標２の中心角*/

double _geo_radmod(double );  /*角度を0～2piの範囲に収める*/
double _geo_radmod2(double ); /*角度を-pi～piの範囲に収める*/
double _geo_degmod(double );  /*角度を0～360の範囲に収める*/
double _geo_degmod2(double ); /*角度を-180～180の範囲に収める*/

double _geo_qatan(double ,double ); /*x座標とy座標のなす角を求める*/

double _geo_distance( /*２点間の距離を返す関数*/
double  ,
double  ,
double  ,
double );

int _geo_rectaco_dirco( /*直交座標->方向余弦の変換関数*/
VECT  ,
VECT *);

int _geo_rectaco_polaco( /*直交座標->極座標の変換関数*/
VECT  ,
VECT *);

int _geo_ginit(void); //グラフィック初期化

int _geo_polaco_rectaco( //極座標->直交座標の変換関数
VECT  ,
VECT *);

XY _geo_RotateRefPoint( //ある基準点がＸ軸上となるように座標回転
XY     P1,  //  基準線座標
XY     P2,  //  基準副点座標
XY      P);  //  被判定点

void _geo_GetRotateRefPointParam( //基準座標をもとにした座標回転角の取得
XY      P1,  //  基準線座標
XY      P2,  //  基準副点座標
double *q1,
double *q2,
double *q3);
/*
この関数は、球面座標において、P1がx軸上、P2y軸正方向の赤道上となるよう
な座標系パラメータを取得し、これへの回転に必要な回転角を取得します。
*/

XY _geo_SeparateSphere(
XY P1,
XY P2,
XY  P);


double _geo_FoggyCut(
XY P1,
XY P2,
XY  P,
double q,
double w);

double _geo_PolygonCut( //球面多角形内外の判定
XY    P,  //被判定座標
XY   *B1, //境界点１配列
XY   *B2, //境界点２配列
int  *sw,
double q, //foggy分割曲率パラメータr
double w, //foggy分割曲率パラメータw
int num); //画数

double _geo_getpoint_on_sphere( //球面上直線上ポイントの極座標取得
double xin,             //直線上の点の位置(0-1)
double x1,double y1,    //直線の第１点
double x2,double y2,    //直線の第１点
double *xo,             //点の赤経(DEG)
double *yo);            //点の赤緯(DEG)

void _geo_RotateToPoint( //ある点を赤経、赤緯を中心にした座標に変換
double x,       //元x座標(degree)
double y,       //元y座標(degree)
double lons,    //中心赤経(degree)
double lats,    //中心赤緯(degree)
double rotate,  //視野回転角(degree)
double *xo,     //変換後赤経(degree)
double *yo);     //変換後赤緯(degree)


double _geo_getdistangle( //極座標２点の距離角(全角)の取得
double lon1,double lat1,  //点1の座標（DEGREE)
double lon2,double lat2); //点2の座標（DEGREE)

double _geo_getpoint_on_sphere2( //直線上経緯度から赤道座標に変換
double x,   //直線上の点の経度
double y,   //直線上の点の緯度
double x1,double y1,  //直線の第１点
double x2,double y2,  //直線の第１点
double *xo, //点の赤経(DEG)
double *yo);//点の赤緯(DEG)

int _geo_sphere_cross( //球と光線の交点の取得
double radius, //球の半径
VECT START,    //光線の始点座標
VECT  DIR,     //方向ベクトル
VECT *RET);    //交点座標




VECT _geo_rotate_vector_xy( //３次元ベクトルを回転する関数(XY)
VECT VI,        //被回転ベクトルの構造体
double rota);   //回転角


VECT _geo_rotate_vector_xz( //３次元ベクトルを回転する関数(XZ)
VECT VI,        //被回転ベクトルの構造体
double rota);   //回転角

VECT _geo_rotate_vector_yz( //３次元ベクトルを回転する関数(YZ)
VECT VI,        //被回転ベクトルの構造体
double rota);   //回転角


int _geo_rectaco_dirco2( //直交座標->方向余弦の変換関数
VECT *VO);

VECT _geo_PolToRect(double alpa,double delta,double radius);
void _geo_RectToPol(VECT V,double *alpa,double *delta);

void AddVect(VECT *V,VECT VADD);
void DelVect(VECT *V,VECT VADD);

double _geo_vectdistance(VECT V1,VECT V2);

int _geo_sphere_line_sect2( //球と直線の交点座標の取得関数
VECT P1,
VECT P2,
VECT CS,
double radius,
VECT *A1,
VECT *A2);

VECT Vect(double x,double y,double z);

#endif;


#endif
