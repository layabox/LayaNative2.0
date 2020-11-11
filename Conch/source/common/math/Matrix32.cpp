/**
@file			Matrix32.cpp
@brief			
@author			James
@version		1.0
@date			2018_7_23
*/

#include "Matrix32.h"
#include <math.h>

namespace laya
{
	Matrix32::Matrix32()
	{
		m[0] = 1;    m[1] = 0;
		m[2] = 0;    m[3] = 1;
		m[4] = 0;    m[5] = 0;
		bOnlyTrans = true;
		bHasRotate = false;
	}
	Matrix32* Matrix32::clone()
	{
		Matrix32* pOther = new Matrix32();
		pOther->m[0] = m[0];
		pOther->m[1] = m[1];
		pOther->m[2] = m[2];
		pOther->m[3] = m[3];
		pOther->m[4] = m[4];
		pOther->m[5] = m[5];
		pOther->bOnlyTrans = bOnlyTrans;
		pOther->bHasRotate = bHasRotate;
		return pOther;
	}
	void Matrix32::lerp( Matrix32* out,Matrix32* a,Matrix32* b,float k )
	{
		out->m[0]=a->m[0]+(b->m[0]-a->m[0])*k;
		out->m[1]=a->m[1]+(b->m[1]-a->m[1])*k;
		out->m[2]=a->m[2]+(b->m[2]-a->m[2])*k;
		out->m[3]=a->m[3]+(b->m[3]-a->m[3])*k;
		out->m[4]=a->m[4]+(b->m[4]-a->m[4])*k;
		out->m[5]=a->m[5]+(b->m[5]-a->m[5])*k;
	}
	void Matrix32::lerp1(Matrix32* mo,std::vector<float> m1,std::vector<float> m2,float f )
	{
		mo->m[0] = m1[0] + (m2[0] - m1[0]) * f;
		mo->m[1] = m1[1] + (m2[1] - m1[1]) * f;
		mo->m[2] = m1[2] + (m2[2] - m1[2]) * f;
		mo->m[3] = m1[3] + (m2[3] - m1[3]) * f;
		mo->m[4] = m1[4] + (m2[4] - m1[4]) * f;
		mo->m[5] = m1[5] + (m2[5] - m1[5]) * f;
	}
	void Matrix32::fromSRT( Matrix32* mo,float sx,float sy,float r,float tx,float ty )
	{
		float st = sinf(r);
		float ct = cosf(r);
		mo->m[0] = sx * ct;
		mo->m[1] = sx * st;
		mo->m[2] = -sy * st;
		mo->m[3] = sy * ct;
		mo->m[4] = tx;
		mo->m[5] = ty;
		mo->calcTSR();
	}
	void Matrix32::fromRST(Matrix32* mo, float sx,float sy,float r,float tx,float ty )
	{
		float st = sinf(r);
		float ct = cosf(r);
		mo->m[0] = sx * ct;
		mo->m[1] = sy * st;
		mo->m[2] = -sx * st;
		mo->m[3] = sy * ct;
		mo->m[4] = tx;
		mo->m[5] = ty;
		mo->calcTSR();
	}
	void Matrix32::mul( Matrix32* mo, Matrix32* m1, Matrix32* m2 )
	{
		float aa = m1->m[0];
		float ab = m1->m[1];
		float ac = m1->m[2];
		float ad = m1->m[3];
		float atx = m1->m[4];
		float aty = m1->m[5];

		float ba = m2->m[0];
		float bb = m2->m[1];
		float bc = m2->m[2];
		float bd = m2->m[3];
		float btx = m2->m[4];
		float bty = m2->m[5];
			
		mo->m[0] = aa*ba + ab*bc;
		mo->m[1] = aa*bb + ab*bd;
		mo->m[2] = ac*ba + ad*bc;
		mo->m[3] = ac*bb + ad*bd;
		mo->m[4] = ba*atx + bc*aty + btx;
		mo->m[5] = bb*atx + bd*aty + bty;
		mo->calcTSR();
	}
    void Matrix32::mul(float* mo, float* m1, float* m2)
    {
        float aa = m1[0];
        float ab = m1[1];
        float ac = m1[2];
        float ad = m1[3];
        float atx = m1[4];
        float aty = m1[5];

        float ba = m2[0];
        float bb = m2[1];
        float bc = m2[2];
        float bd = m2[3];
        float btx = m2[4];
        float bty = m2[5];

        mo[0] = aa*ba + ab*bc;
        mo[1] = aa*bb + ab*bd;
        mo[2] = ac*ba + ad*bc;
        mo[3] = ac*bb + ad*bd;
        mo[4] = ba*atx + bc*aty + btx;
        mo[5] = bb*atx + bd*aty + bty;
    }
	void Matrix32::identity ()
	{
		this->m[0] = this->m[3] = 1;
		this->m[1] = this->m[2] = this->m[4] = this->m[5] = 0;
		this->bOnlyTrans = true;
		this->bHasRotate = false;
	}
	Matrix32* Matrix32::copy( Matrix32* p_pOther )
	{
		*this=*p_pOther;
		return this;
	}
	bool Matrix32::IsEqual( Matrix32* p_pOther )
	{
		if( m[0] != p_pOther->m[0] || m[1] != p_pOther->m[1] || m[2] != p_pOther->m[2] || m[3] != p_pOther->m[3] || m[4] != p_pOther->m[4] || m[5] != p_pOther->m[5]  )
		{
			return false;
		}
		return true;
	}
	void Matrix32::translate (float x, float y )
	{
		if( bOnlyTrans )
        {
            this->m[4] += x;
            this->m[5] += y;
        }
        else
        {
            this->m[4] = x*this->m[0] + y*this->m[2] + this->m[4];
            this->m[5] = x*this->m[1] + y*this->m[3] + this->m[5];
        }
	}
    void Matrix32::translate(float* m,float x, float y)
    {
        m[4] = x*m[0] + y*m[2] + m[4];
        m[5] = x*m[1] + y*m[3] + m[5];   
    }
    void Matrix32::scale(float* m, float x, float y)
    {
        m[0] *= x;
        m[1] *= y;
        m[2] *= x;
        m[3] *= y;
    }
    void Matrix32::rotate(float* m, float angle)
    {
        if (angle == 0)return;
        float tmp1 = cosf(angle);
        float tmp2 = sinf(angle);
        float tm11 = tmp1*m[0] + tmp2*m[2];
        float tm12 = tmp1*m[1] + tmp2*m[3];
        float tm21 = -tmp2*m[0] + tmp1*m[2];
        float tm22 = -tmp2*m[1] + tmp1*m[3];
        m[0] = tm11;
        m[1] = tm12;
        m[2] = tm21;
        m[3] = tm22;
    }
	void Matrix32::scale_rotate( float arc,float x,float y )
	{
		if ( arc == 0 && x == 1 && y == 1 ) return;
		if ( arc == 0 )
		{
			m[0] *= x;
			m[1] *= x;
			m[2] *= y;
			m[3] *= y;
			bOnlyTrans = false;
			bHasRotate = false;
		}
		else
		{
			float tmp1 =  cosf(arc); 
			float tmp2 = sinf(arc);
			float tm11 = tmp1*m[0] + tmp2*m[2];
			float tm12 = tmp1*m[1] + tmp2*m[3];
			float tm21 = -tmp2*m[0] + tmp1*m[2];
			float tm22 = -tmp2*m[1] + tmp1*m[3];
			m[0] = tm11 * x; 
			m[1] = tm12 * x;
			m[2] = tm21 * y;
			m[3] = tm22 * y;
			bOnlyTrans = false;
			bHasRotate = true;
		}
	}
	void Matrix32::rotate ( float angle )
	{
		if( angle==0)return;
		float tmp1 =  cosf(angle); 
		float tmp2 = sinf(angle);
		float tm11 = tmp1*m[0] + tmp2*m[2];
		float tm12 = tmp1*m[1] + tmp2*m[3];
		float tm21 = -tmp2*m[0] + tmp1*m[2];
		float tm22 = -tmp2*m[1] + tmp1*m[3];
		m[0] = tm11; 
		m[1] = tm12;
		m[2] = tm21;
		m[3] = tm22;
		bOnlyTrans = false;
		bHasRotate = true;
	}
	void Matrix32::scale (float sx,float sy )
	{
		if( sx==1 && sy==1 ) return;
		m[0] *= sx;
		m[1] *= sx;
		m[2] *= sy;
		m[3] *= sy;
		bOnlyTrans = false;
	}
	void Matrix32::setTransform( float n11,float n12,float n21,float n22,float n31,float n32 )
	{
		m[0] = n11;
		m[1] = n12;
		m[2] = n21;
		m[3] = n22;
		m[4] = n31;
		m[5] = n32;
		calcTSR();
	}
	void Matrix32::transform( float n11,float n12,float n21,float n22,float n31,float n32 )
	{
		////m * n = n * p;  m矩阵的行 * n矩阵的列
		//Number k11 = m11 * n11 + m12 * n21 + 0 * n31;
		//Number k12 = m11 * n12 + m12 * n22 + 0 * n32;
		//Number k21 = m21 * n11 + m22 * n21 + 0 * n31;
		//Number k22 = m21 * n12 + m22 * n22 + 0 * n32;
		//Number k31 = m31 * n11 + m32 * n21 + 1 * n31;
		//Number k32 = m31 * n12 + m32 * n22 + 1 * n32;
			
		//m * n = n * p;  m矩阵的行 * n矩阵的列
        /*
		float k11 = n11 * m[0] + n12 * m[2] + 0 * m[4];
		float k12 = n11 * m[1] + n12 * m[3] + 0 * m[5];
		float k21 = n21 * m[0] + n22 * m[2] + 0 * m[4];
		float k22 = n21 * m[1] + n22 * m[3] + 0 * m[5];
		float k31 = n31 * m[0] + n32 * m[2] + 1 * m[4];
		float k32 = n31 * m[1] + n32 * m[3] + 1 * m[5];
        */

        float k11 = n11 * m[0] + n12 * m[2];
        float k12 = n11 * m[1] + n12 * m[3];
        float k21 = n21 * m[0] + n22 * m[2];
        float k22 = n21 * m[1] + n22 * m[3];
        float k31 = n31 * m[0] + n32 * m[2] + m[4];
        float k32 = n31 * m[1] + n32 * m[3] + m[5];
		m[0] = k11;
		m[1] = k12;
		m[2] = k21;
		m[3] = k22;
		m[4] = k31;
		m[5] = k32;
        bOnlyTrans=false;
        bHasRotate=true;
	}
	void Matrix32::invert ()
	{
		float a =  m[0];
		float b =  m[1];
		float c =  m[2];
		float d =  m[3];
		float tx =  m[4];
		float i = a * d - b * c;			
		this->m[0] = d / i;
		this->m[1] = -b / i;
		this->m[2] = -c / i;
		this->m[3] = a / i;
		this->m[4] = (c * this->m[5] - d * tx) / i;
		this->m[5] = -(a * this->m[5] - b * tx) / i;
		bOnlyTrans = false;
	}
	void Matrix32::calcTSR()
	{
		if( m[0] == 1 && m[1] == 0 && m[2] == 0 && m[3] == 1 )
		{
			bOnlyTrans = true;
			bHasRotate = false;
		}
		else
		{
			bOnlyTrans = false;
			if( fabsf( m[1] ) > 0.0001 || fabsf( m[2] ) > 0.0001 )
			{
				bHasRotate = true;
			}
			else
			{
				bHasRotate = false;
			}
		} 
	}
	void Matrix32::concat ( Matrix32* mtx )
	{
		float a =  m[0];
		float c =  m[2];
		float tx =  m[4];
		m[0] = a * mtx->m[0] + this->m[1] * mtx->m[2];
		m[1] = a * mtx->m[1] + this->m[1] * mtx->m[3];
		m[2] = c * mtx->m[0] + this->m[3] * mtx->m[2];
		m[3] = c * mtx->m[1] + this->m[3] * mtx->m[3];
		m[4] = tx * mtx->m[0] + this->m[5] * mtx->m[2] + mtx->m[4];
		m[5] = tx * mtx->m[1] + this->m[5] * mtx->m[3] + mtx->m[5];
		calcTSR();
	}
	bool Matrix32::isIdentity()
	{
		return (m[0] == 1 && m[1] == 0 && m[2] == 0 && m[3] == 1 && m[4] == 0 && m[5] == 0 );
	}
    void Matrix32::transVert(float x, float y, float& ox, float& oy) 
    {
        ox = x*m[0] + y*m[2] + m[4];
        oy = x*m[1] + y*m[3] + m[5];
    }
    void Matrix32::skew(float x, float y)
    {
        float sinx = sinf(-x);
        float cosx = cosf(x);
        float siny = sinf(y);
        float cosy = cosf(y);
        float a = m[0], b = m[1], c = m[2], d = m[3], tx = m[4], ty = m[5];
        m[0] = a*cosy - b*sinx;
        m[1] = a*siny + b*cosx;
        m[2] = c*cosy - d*sinx;
        m[3] = c*siny + d*cosx;
        m[4] = tx*cosy - ty*sinx;
        m[5] = tx*siny + ty*cosx;
        calcTSR();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
