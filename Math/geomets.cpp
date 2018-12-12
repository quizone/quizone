
#include"geomets.h"


#ifndef PI
#define   PI  3.14159265358979
#endif

#ifndef PI2
#define  PI2  6.28318530717959
#endif

Equal(double Value1, double Value2)
{
if (fabs(Value1-Value2) > 0.000001) return 0;
return 1;
}

static void fswap(double *,double *);  /*実数の入替関数*/
static void rotate(double ,XY ,XY ,XY *);
static int _geo_reflact2(double ,double ,double *);

int _geo_circlevector_sect (/*円とベクトルの交差判定関数*/
XY      P0,   /*ベクトルの始点座標  */
double   q,   /*ベクトルの角度      */
double   r,   /*曲率半径        */
XY     XYC,   /*半径の中心座標  */
XY     *P1,   /*交点座標１=Ｘ座標の手前方*/
XY     *P2,   /*交点座標２=Ｘ座標の向こう方*/
double *q1,   /*交点座標１の中心角*/
double *q2)   /*交点座標２の中心角*/
{
XY  G1,GO1,GO2;  /*中間座標*/
XY  GOS1,GOS2;   /*中間座標*/
double a,b;
double aa,bb,cc;
double flag;


	G1.x=P0.x-XYC.x; /*円心座標に変換*/
	G1.y=P0.y-XYC.y;

	a= sin(q)/ cos(q);  /*傾き*/
	b=G1.y-G1.x*a;    /*切片*/

	aa=1+a*a;
	bb=2*a*b;
	cc=b*b-r*r;

	flag=bb*bb-4*aa*cc;

	if(flag<0) return 0;  /*交点がない*/

	GO1.x=(-bb+sqrt(flag))/(2.0*aa);
	GO2.x=(-bb-sqrt(flag))/(2.0*aa);

	GO1.y=a*GO1.x+b;
	GO2.y=a*GO2.x+b;

	rotate(q,GO1,XYC,&GOS1);   /*回転後座標*/
	rotate(q,GO2,XYC,&GOS2);   /*回転後座標*/

	if(GOS1.x>GOS2.x)  /*解１の方が先方にある場合は入れ替える*/
	{
		fswap(&GO1.x,&GO2.x);
		fswap(&GO1.y,&GO2.y);
	}

	*q1=_geo_qatan(GO1.y,GO1.x);     /*交点１の中心角を求める*/
	*q2=_geo_qatan(GO2.y,GO2.x);     /*交点２の中心角を求める*/

	P1->x=GO1.x+XYC.x;
	P1->y=GO1.y+XYC.y;
	P2->x=GO2.x+XYC.x;
	P2->y=GO2.y+XYC.y;

	return 1;
}


double _geo_degmod(double a) /*角度を0～2PIの範囲に収める*/
{
double b;

	if(a>=0.0) b=fmod(a,360.0);
	else       b=360.0+fmod(a,360.0);

       //	if(b==360.0) b=0.0;
       if (fabs(b-360) < 0.0001) b=0.0;

return b;
}


double _geo_distance( /*平面２点間の距離を返す関数*/
double x1,
double y1,
double x2,
double y2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


double _geo_radmod(double a) /*角度を0～2PIの範囲に収める*/
{
	if(a>=0.0) return fmod(a,PI2);
	else return PI2+fmod(a,PI2);
}

double _geo_radmod2(double a) /*角度を-PI～PIの範囲に収める*/
{
double b;
	b=_geo_radmod(a);
	if(b>=PI) b=b-PI2;
return b;
}


double _geo_degmod2(double a) /*角度を-PI～PIの範囲に収める*/
{
double b;
	b=_geo_degmod(a);
	if(b>=180.0) b=b-360.0;
return b;
}


double _geo_qatan(double y,double x) /*x座標とy座標のなす角を求める*/
{
	if(Equal(x,0.0))
	{
		if(y>0.0) return PI/2.0; else return 3*PI/2.0;
	}
      	else if(x>0.0) return _geo_radmod(atan(y/x));
      	else           return _geo_radmod(atan(y/x)+PI);
}


int _geo_rectaco_dirco( //直交座標->方向余弦の変換関数
VECT VI,
VECT *VO)
{
double dis;

	dis=sqrt(VI.x*VI.x + VI.y*VI.y + VI.z*VI.z); //距離
	if(dis==0.0) return 0;
	VO->x=VI.x/dis; VO->y=VI.y/dis; VO->z=VI.z/dis;

return 1;
}


int _geo_rectaco_dirco2( //直交座標->方向余弦の変換関数
VECT *VO)
{
    double dis;
    VECT VI;

    VI = *VO;

	dis=sqrt(VI.x*VI.x + VI.y*VI.y + VI.z*VI.z); //距離
	if(dis==0.0) return 0;
	VO->x=VI.x/dis; VO->y=VI.y/dis; VO->z=VI.z/dis;

return 1;
}


int _geo_rectaco_polaco( //直交座標->極座標の変換関数
VECT VI,
VECT *VO)
{
    VO->x=0.0;
    VO->y=0.0;
    VO->z=0.0;
    VO->z=sqrt(VI.x*VI.x + VI.y*VI.y + VI.z*VI.z); //距離

   	if(VO->z==0.0) return 0; //距離が0のときは計算不能で0を返す
    	VO->x=_geo_qatan(VI.y,VI.x);
	//if(Equal(VI.x,0.0) && (Equal(VI.y,0.0))) //極点のとき
    if ((VI.x==0.0) && (VI.y==0.0))
	{
		if(VI.z>0.0) { VO->y= PI/2.0; VO->x=0.0; return 1; }
		else         { VO->y=-PI/2.0; VO->x=0.0; return 1; }
	}
    	VO->y= atan(VI.z/sqrt(VI.x*VI.x+VI.y*VI.y));
    return 1;
}


void _geo_RectToPol(VECT V,double *alpa,double *delta)
{
    VECT VO;
    if (_geo_rectaco_polaco(V,&VO) ==0)
        int h=0;
    *alpa  = VO.x;
    *delta = VO.y;
    return ;
}


int _geo_polaco_rectaco( //極座標->直交座標の変換関数
VECT VI,
VECT *VO)
{
	//引数構造体のz成分を距離として直交座標を取得する

	VO->x=VI.z* cos(VI.x)* cos(VI.y);
	VO->y=VI.z* sin(VI.x)* cos(VI.y);
	VO->z=VI.z* sin(VI.y);
return 0;
}


VECT _geo_PolToRect(double alpa,double delta,double radius)
{
   static double ca = 0.05;
   static double sa = 0.05;
   static double cd = 0.05;
   static double sd = 0.05;
   static double alpa1 = 0.05;
   static double delta1 = 0.05;

    if(delta1 != delta)
    {
        cd = cos(delta);
        sd = sin(delta);
        delta1 = delta;
    }
    if(alpa1 != alpa)
    {
        ca = cos(alpa);
        sa = sin(alpa);
        alpa1 = alpa;
    }


	//極座標から直交座標を取得する
    VECT V;

	V.x = radius * ca * cd;
	V.y = radius * sa * cd;
	V.z = radius * sd;
    return V;

}


int _geo_reflact( /*屈折計算関数*/
double k,    /*屈折率*/
double q,    /*入射角*/
double *qoo) /*出射角*/
{
double qo,qq;
int flag=0;

	q=fmod(q+100.0*PI,2*PI);
	if(q>PI) q=q-2*PI;

	if(q>=-PI2 && q<=PI2) /*直角以内*/
	{
		if(k<0.0) { *qoo= PI-q; return 0; }  /*反射した*/
		flag=_geo_reflact2(k,q,&qo);
		if(flag==-1) { *qoo=PI-q; return 0; }  /*全反射*/
		else         { *qoo=  qo; return 1; }  /*屈折した*/
	}
	else /*直角以上のとき*/
	{
		if(k<0.0) { *qoo= PI-q; return 0; }  /*反射した*/
		qq=PI-q;
		flag=_geo_reflact2(1/k,qq,&qo);
		if(flag==-1) { *qoo=PI-q; return 0; }   /*全反射した*/
		else         { *qoo=PI-qo;return 1; }   /*屈折した*/
	}

}


_geo_reflact_h(     /*任意法線に対する屈折計算*/
double k,   /*屈折率*/
double qi,  /*入射角*/
double qh,  /*法線角*/
double *qo) /*出力角*/
{
double dqi,dqo;
int flag;

	dqi=_geo_radmod(qi-qh+PI);
	flag=_geo_reflact(k,dqi,&dqo);
	*qo=_geo_radmod(dqo+qh-PI);

	return flag;
}


void _geo_rotate_vector( //３次元ベクトルを回転する関数
VECT *q,  //被回転ベクトルの構造体
VECT *a,  //回転後ベクトルの構造体
double rota_a,     //第一回転角
double rota_b,     //第２回転角
double rota_c)     //第３回転角
{
VECT j1; //中間ベクトル構造体１
VECT j2; //中間ベクトル構造体２
	j1.x=+q->x* cos(rota_a) +q->y* sin(rota_a);
	j1.y=-q->x* sin(rota_a) +q->y* cos(rota_a);
	j1.z= q->z;
	j2.x=+j1.x* cos(rota_b) +j1.z* sin(rota_b);
	j2.y=j1.y;
	j2.z=-j1.x* sin(rota_b) +j1.z* cos(rota_b);
	a->x=+j2.x;
	a->y=+j2.y* cos(rota_c) +j2.z* sin(rota_c);
		a->z=-j2.y* sin(rota_c) +j2.z* cos(rota_c);
}


void _geo_rotatexy( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,     //回転角正弦
double a_cos)     //回転角余弦
{
    double x,y,z;

	x =   V->x * a_cos + V->y * a_sin;
	y =  -V->x * a_sin + V->y * a_cos;

    V->x = x;
    V->y = y;
    return ;
}


void _geo_rotatexz( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,     //回転角正弦
double a_cos)     //回転角余弦
{
    double x,y,z;

	x =   V->x * a_cos + V->z * a_sin;
	z =  -V->x * a_sin + V->z * a_cos;

    V->x = x;
    V->z = z;
    return ;
}


void _geo_rotateyz( //３次元ベクトルを回転する関数
VECT *V,  //被回転ベクトルの構造体
double a_sin,     //回転角正弦
double a_cos)     //回転角余弦
{
    double x,y,z;

	y =   V->y * a_cos + V->z * a_sin;
	z =  -V->y * a_sin + V->z * a_cos;

    V->y = y;
    V->z = z;
    return ;
}



static int _geo_reflact2(  /*屈折計算内部関数（法線角直角以内）*/
double k,
double q,
double *qo)
{
double sk;

	sk= sin(q)/k;
	if(fabs(sk)>1.0) return -1;  /*反射した場合*/
	*qo = asin(sk);
	return 1;
}


int _geo_sphere_line_sect( //球と直線の交点座標の取得関数
DVECT *vector_point,   //直線の通過座標（２点）
DVECT *inter_point,    //交点座標
VECT   sphere_center,  //球の中心座標
double  radius )       //球の半径
//返値  実数解:1 / 虚数解:-1
{
double x1,y1,z1,x2,y2,z2;       //入力ベクトル座標の格納変数
double xc,yc,zc;                //球の中心座標
double a,b,c,d;                 //直線１次関数のパラメータ
double aa,bb,cc;                //交点獲得２次方程式の係数
double ha;                      //虚数／実数解  判別式数
double ax1,ay1,az1,ax2,ay2,az2; //球を中心とした出力座標の格納変数

	x1=vector_point->x1;
	y1=vector_point->y1;
	z1=vector_point->z1;

		//中心座標を構造体からローカル変数へ
	x2=vector_point->x2; y2=vector_point->y2; z2=vector_point->z2;
	 xc=sphere_center.x;  yc=sphere_center.y;  zc=sphere_center.z;
	if(x2!=x1) a=(y2-y1)/(x2-x1);
	else a=(y2-y1)/0.0000001;
	b=y1-a*x1;
	if(x2!=x1) c=(z2-z1)/(x2-x1);
	else c=(z2-y1)/0.0000001;
	d=z1-c*x1;

	aa=1+a*a+c*c;
	bb=2*(a*b +c*d -xc -yc*a -zc*c);
	cc=xc*xc +yc*yc +zc*zc +b*b +d*d -2*yc*b -2*zc*d -radius*radius;
	ha=bb*bb-4*aa*cc;
	if(ha<0.0)
	{
		inter_point->x1=0.0;
		inter_point->y1=0.0;
		inter_point->z1=0.0;

		inter_point->x2=0.0;
		inter_point->y2=0.0;
		inter_point->z2=0.0;

		return 0;   //虚数解で、座標はすべて０とする
	}

	else
	{
		ax1=(-bb+sqrt(ha))/(2*aa);
		ay1=a*ax1+b;
		az1=c*ax1+d;
		ax2=(-bb-sqrt(ha))/(2*aa);
		ay2=a*ax2+b;
		az2=c*ax2+d;

		if(x1 > x2)
		{
			inter_point->x1=ax1;
			inter_point->y1=ay1;
			inter_point->z1=az1;

			inter_point->x2=ax2;
			inter_point->y2=ay2;
			inter_point->z2=az2;
		}
		else
		{
			inter_point->x1=ax2;
			inter_point->y1=ay2;
			inter_point->z1=az2;

			inter_point->x2=ax1;
			inter_point->y2=ay1;
			inter_point->z2=az1;
		}
		return 1;  //実数解
	}
}


static void fswap(double *a,double *b)  /*実数の入替関数*/
{
double c;
	c=*a;  *a=*b; *b=c;
}

static void rotate(  /*座標系の回転関数*/
double q,  /*回転角*/
XY  V1  ,  /*回転前座標*/
XY  VS  ,  /*回転中心座標*/
XY *V2)    /*回転後座標*/
{
	V2->x= + cos(q)*(V1.x-VS.x) + sin(q)*(V1.y-VS.y) + VS.x;
	V2->y= - sin(q)*(V1.x-VS.x) + cos(q)*(V1.y-VS.y) + VS.y;
}


XY _geo_RotateRefPoint( //基準座標をもとにした座標回転の実行
XY     P1,  //  基準線座標
XY     P2,  //  基準副点座標
XY      P)  //  被判定点
/*
この関数は、球面座標において、P1がx軸上、P2y軸正方向の赤道上となるよう
な座標系パラメータを取得し、Pの座標をこの新しい座標に変換します。
返値は、取得したPの変換座標です。
*/
{
	VECT VI,VO;
	XY XYRET;
	double q1,q2,q3;

	VO.x = P.x; VO.y = P.y; VO.z=1.0;
	_geo_polaco_rectaco(VO,&VI); //始点座標を方向余弦に変換
	_geo_GetRotateRefPointParam(P1,P2,&q1,&q2,&q3);
	_geo_rotate_vector(&VI,&VO,q1,q2,q3);  //座標の回転
	_geo_rectaco_polaco(VO,&VI);
	XYRET.x = VI.x;
	XYRET.y = VI.y;

	return XYRET;
}

void _geo_GetRotateRefPointParam( //基準座標をもとにした座標回転角の取得
XY      P1,  //  基準線座標
XY      P2,  //  基準副点座標
double *q1,
double *q2,
double *q3)
/*
この関数は、球面座標において、P1がx軸上、P2y軸正方向の赤道上となるよう
な座標系パラメータを取得し、これへの回転に必要な回転角を取得します。
*/
{
	VECT VTMP,VP1,VP2,VPO2;

	VTMP.x = P1.x; VTMP.y = P1.y; VTMP.z=1.0;
	_geo_polaco_rectaco(VTMP,&VP1); //始点座標を方向余弦に変換
	VTMP.x = P2.x; VTMP.y = P2.y; VTMP.z=1.0;
	_geo_polaco_rectaco(VTMP,&VP2); //始点座標を方向余弦に変換
	*q1 =_geo_qatan(VP1.y,VP1.x);   //水平回転角の取得
	*q2 =asin(VP1.z);               //上下回転角の取得
	_geo_rotate_vector(&VP2,&VPO2,*q1,*q2,0.0); //まずP1をx軸に一致
	*q3 =_geo_qatan(VPO2.z,VPO2.y);   //P2を赤道一致させる回転角の取得
	return;
}

XY _geo_SeparateSphere(
XY P1,
XY P2,
XY  P)
/*
この関数は、極座標で与えられる球面上の２点を結ぶ線分と、任意の点の間
の位置関係を判定します。
*/
{
	XY PP1,PP2,CENT,PPTMP,PPRET;
	double q;

	PP1 = _geo_RotateRefPoint(P1,P2,P);  //対象点を座標変換
	PP2 = _geo_RotateRefPoint(P1,P2,P2); //副基準点を座標変換
	q = PP2.x/2.0;    //主副間中心がx軸となる水平回転角の取得
	CENT.x=0.0; CENT.y=0.0;
	PPTMP.x=cos(PP1.x);
	PPTMP.y=sin(PP1.x);
	rotate(q,PPTMP,CENT,&PPRET);  //主副中心=ｘ軸への回転
	PP1.x = _geo_radmod2(_geo_qatan(PPRET.y,PPRET.x)); //対象点水平角
	PP1.x = -fabs(PP1.x) * 1.0/q+1.0;
//	if(PP1.y<0.0) PP1.y=0.0; else PP1.y=1.0;
	return PP1;
}

double _geo_FoggyCut(
XY P1,
XY P2,
XY  P,
double q,
double w)
{
	XY PP;
	double b,r,ax,h,hh,k,ret;

	PP = _geo_SeparateSphere(P1,P2,P);

	if(PP.x>0.0 && fabs(q)!=0.0 && w!=0.0)
	{
		r  = 1.0/sin(q);
		b  = r * cos(q);
		ax = (1.0-PP.x);
		h  = sqrt(r*r-ax*ax) - b;
		k  = w/(r*(1.0-cos(q)));
		hh = h * k;
		if(hh!=0.0) ret = PP.y/hh;
		else        ret =1.0;

		if(ret>1.0) ret=1.0;
		if(ret<-1.0) ret=-1.0;
		ret = ret * 0.5 + 0.5;
	}
	else
	{
		if(PP.y>0.0) ret = 1.0;
		else         ret = 0.0;
	}
	return ret;
}


double _geo_PolygonCut( //球面多角形内外の判定
XY    P,  //被判定座標
XY   *B1, //境界点１配列
XY   *B2, //境界点２配列
int  *sw, //enable switch
double q, //foggy分割曲率パラメータq
double w, //foggy分割曲率パラメータw
int num)  //画数
/*
この関数は、B1-B2で定義される境界線に囲まれたデータを判定します。
*/
{
	int n;
	double ret=1.0;

	for(n=0; n<num;n++)
	{
		if(sw[n]) ret*=_geo_FoggyCut(B1[n],B2[n],P,q,w);
	}
	return ret;
}



double _geo_getpoint_on_sphere( //球面上直線上ポイントの座標取得
double xin,   //直線上の点の位置(0-1)
double x1,double y1,  //直線の第１点
double x2,double y2,  //直線の第１点
double *xo, //点の赤経(DEG)
double *yo) //点の赤緯(DEG)
{
    double dist;

    dist = _geo_getdistangle(x1,y1,x2,y2);

    return _geo_getpoint_on_sphere2(xin * dist,0.0,x1,y1,x2,y2,xo,yo);
}


double _geo_getpoint_on_sphere2( //直線上経緯度から赤道座標に変換
double x,   //直線上の点の経度
double y,   //直線上の点の緯度
double x1,double y1,  //直線の第１点
double x2,double y2,  //直線の第１点
double *xo, //点の赤経(DEG)
double *yo) //点の赤緯(DEG)
/*
この関数は、球面上の端点１(x1,y1)と端点２(x2,y2)を結ぶ直線を基準とする
極座標から赤道座標を取得する。直線上の位置は、経度、緯度で指定する。
端点１は、元座標の原点（赤道座標でいうところの春分点）であり、端点２は
赤道上の正方向点である。取得された値はポインタxo,yoに格納される。
戻値は、2点の角距離（degree)である。
*/
{
    double q1,q2,q3,q4; //座標変換回転角(rad)

    VECT ECX1; //切断線端点X1の赤道座標(RAD)
    VECT ECX2; //切断線端点X2の赤道座標(RAD)
    VECT DCX1_1; //切断線端点X1の赤道方向余弦(RAD)
    VECT DCX1_2; //切断線端点X1のq1回転後の赤道方向余弦(RAD)
    VECT DCX1_3; //切断線端点X1のq2回転後の赤道方向余弦(RAD)
    VECT DCX1_4; //切断線端点X1のq3回転後の赤道方向余弦(RAD)
    VECT DCX1_5; //切断線端点X1のq4回転後の赤道方向余弦(RAD)

    VECT DCX2_1; //切断線端点X2の赤道方向余弦(RAD)
    VECT DCX2_2; //切断線端点X2のq1回転後の赤道方向余弦(RAD)
    VECT DCX2_3; //切断線端点X2のq2回転後の赤道方向余弦(RAD)
    VECT DCX2_4; //切断線端点X2のq3回転後の赤道方向余弦(RAD)
    VECT DCX2_5; //切断線端点X2のq4回転後の赤道方向余弦(RAD)

    VECT PC1,PC2,PC3,PC4,PC5,PC6;

    ECX1.x = x1 * DTOR;
    ECX1.y = y1 * DTOR;
    ECX1.z = 1.0;

    ECX2.x = x2 * DTOR;
    ECX2.y = y2 * DTOR;
    ECX2.z = 1.0;

    _geo_polaco_rectaco(ECX1,&DCX1_1); //方向余弦に変換
    _geo_polaco_rectaco(ECX2,&DCX2_1); //方向余弦に変換

    q1 = _geo_qatan(DCX1_1.y,DCX1_1.x); //座標回転第一角
    DCX1_2 = _geo_rotate_vector_xy(DCX1_1,q1); //
    DCX2_2 = _geo_rotate_vector_xy(DCX2_1,q1); //
    q2 = _geo_qatan(DCX1_2.z,DCX1_2.x); //座標回転第二角
    DCX1_3 = _geo_rotate_vector_xz(DCX1_2,q2); //
    DCX2_3 = _geo_rotate_vector_xz(DCX2_2,q2); //
    q3 = _geo_qatan(DCX2_3.z,DCX2_3.y); //座標回転第三角
    DCX1_4 = _geo_rotate_vector_yz(DCX1_3,q3); //
    DCX2_4 = _geo_rotate_vector_yz(DCX2_3,q3); //
    q4 = _geo_qatan(DCX2_4.y,DCX2_4.x); //線の長さ

    PC1.x = x * DTOR; //点の境界線上座標を極座標で
    PC1.y = y * DTOR;
    PC1.z = 1.0;
    _geo_polaco_rectaco(PC1,&PC2); //方向余弦に変換
    PC3 = _geo_rotate_vector_yz(PC2,-q3); //
    PC4 = _geo_rotate_vector_xz(PC3,-q2); //
    PC5 = _geo_rotate_vector_xy(PC4,-q1); //

    _geo_rectaco_polaco(PC5,&PC6); //極座標に変換

    *xo = PC6.x * RTOD;
    *yo = PC6.y * RTOD;
    return q4 * RTOD;

}


/*
double _geo_getdistangle( //極座標２点の距離角(全角)の取得（こちらが旧型）
double lon1,double lat1, //点1の座標（DEGREE)
double lon2,double lat2) //点2の座標（DEGREE)
{
    VECT VP1,VP2,VC1,VC2;
    double dx,dy,dz,dist;

    VP1.x = lon1 * DTOR;
    VP1.y = lat1 * DTOR;
    VP1.z = 1.0;
    VP2.x = lon2 * DTOR;
    VP2.y = lat2 * DTOR;
    VP2.z = 1.0;

    _geo_polaco_rectaco(VP1,&VC1);
    _geo_polaco_rectaco(VP2,&VC2);

    dx = VC1.x - VC2.x;
    dy = VC1.y - VC2.y;
    dz = VC1.z - VC2.z;

    dist = sqrt(dx * dx + dy * dy + dz * dz) / 2.0;
    return asin(dist) * 2.0 * RTOD;
}

*/
double _geo_getdistangle( //極座標２点の距離角(全角)の取得 (こちらが新形）
double lon1,double lat1, //点1の座標（DEGREE)
double lon2,double lat2) //点2の座標（DEGREE)
{
    VECT VP1,VP2,VC1,VC2;
    double dx,dy,dz,dist;

    VP1 = _geo_PolToRect(lon1 * DTOR,lat1 * DTOR,1.0);
    VP2 = _geo_PolToRect(lon2 * DTOR,lat2 * DTOR,1.0);

    return _geo_getvectdistangle(VP1,VP2) * RTOD;

}



double _geo_getvectdistangle( //直交座標２点の距離角(全角)の取得
VECT VP1,VECT VP2)
{
    VECT VC1,VC2;
    double q1,q2,lon,lat;

    _geo_rectaco_dirco(VP1,&VP1);
    _geo_rectaco_dirco(VP2,&VP2);

    q1 = _geo_qatan(VP1.y,VP1.x);
    VP1 = _geo_rotate_vector_xy(VP1,q1);
    VP2 = _geo_rotate_vector_xy(VP2,q1);
    q2 = _geo_qatan(VP1.z,VP1.x);
    VP1 = _geo_rotate_vector_xz(VP1,q2 - PI / 2.0);
    VP2 = _geo_rotate_vector_xz(VP2,q2 - PI / 2.0);
    _geo_RectToPol(VP2,&lon,&lat);
    return PI / 2.0 - lat;
}


VECT _geo_rotate_vector_xy( //３次元ベクトルを回転する関数(XY)
VECT VI,        //被回転ベクトルの構造体
double rota)   //回転角
{
    static double q = 1.0e+09;
    VECT VO; //中間ベクトル構造体１

	VO.x=+VI.x* cos(rota) +VI.y* sin(rota);
	VO.y=-VI.x* sin(rota) +VI.y* cos(rota);
	VO.z= VI.z;
    q = rota;
    return VO;
}




VECT _geo_rotate_vector_xz( //３次元ベクトルを回転する関数(XZ)
VECT VI,        //被回転ベクトルの構造体
double rota)   //回転角
{
VECT VO; //中間ベクトル構造体１

	VO.x =  VI.x* cos(rota) +VI.z* sin(rota);
	VO.y =  VI.y;
	VO.z = -VI.x* sin(rota) +VI.z* cos(rota);
    return VO;
}


VECT _geo_rotate_vector_yz( //３次元ベクトルを回転する関数(YZ)
VECT VI,        //被回転ベクトルの構造体
double rota)   //回転角
{
VECT VO; //中間ベクトル構造体１

	VO.x=+VI.x;
	VO.y=+VI.y* cos(rota) +VI.z* sin(rota);
	VO.z=-VI.y* sin(rota) +VI.z* cos(rota);
    return VO;
}

void AddVect(VECT *V,VECT VADD)
{
    V->x += VADD.x;
    V->y += VADD.y;
    V->z += VADD.z;
    return ;
}

void DelVect(VECT *V,VECT VADD)
{
    V->x -= VADD.x;
    V->y -= VADD.y;
    V->z -= VADD.z;
    return ;
}


double _geo_vectdistance(VECT V1,VECT V2)
{
    double dx,dy,dz;

    dx = V1.x - V2.x;
    dy = V1.y - V2.y;
    dz = V1.z - V2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}


VECT Vect(double x,double y,double z)
{
    VECT V;
    V.x = x;
    V.y = y;
    V.z = z;
    return V;
}


XY Xy(double x,double y)
{
    XY V;
    V.x = x;
    V.y = y;
    return V;
}


int _geo_sphere_line_sect2( //球と直線の交点座標の取得関数
VECT P1,
VECT P2,
VECT CS,
double radius,
VECT *A1,
VECT *A2)
{
    DVECT D,DR;
    int ret;

    D.x1 = P1.x;
    D.y1 = P1.y;
    D.z1 = P1.z;
    D.x2 = P2.x;
    D.y2 = P2.y;
    D.z2 = P2.z;
    ret = _geo_sphere_line_sect(&D,&DR,CS,radius);
    A1->x = DR.x1;
    A1->y = DR.y1;
    A1->z = DR.z1;
    A2->x = DR.x2;
    A2->y = DR.y2;
    A2->z = DR.z2;
    return ret;
}

int _geo_SepaHMS(double angle,int *hour,int *min,double *sec)
{
    int dir = 0;

    if(angle < 0.0)
    {
        angle = - angle;
        dir = 1;
    }
    *hour = angle;
    *min = (double)(angle - *hour) * 60.0;
    *sec = (angle - (double)*hour  - (double)*min / 60.0) * 3600.0;

    if(dir)
    {
        *hour =  - *hour;
        return false;
    }
    else
    {
        return true;
    }

}



