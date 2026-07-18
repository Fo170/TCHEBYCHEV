#pragma once
/* Approximation des fonctions (Methode de TCHEBYCHEV)
   v = 2.(x-Xmin)/(Xmax-Xmin) - 1
   o = Acos(v) , cos(K.o)=Tk(v)
   f(x) = Sum[k=0 a n]( Ak.Tk(v) )
*/
#include "Types.h"

class TCHBYCHV
{
public:
    int Np;
    Ldbl Xmin, Xmax;
    Ldbl *aT, *X, *Y;

    TCHBYCHV(int NP)
    {
        Np = NP;
        aT = new Ldbl[NP];
        X  = new Ldbl[NP];
        Y  = new Ldbl[NP];
        for (int i = 0; i < NP; i++)
            aT[i] = X[i] = Y[i] = 0.0L;
    }

    ~TCHBYCHV()
    {
        delete[] aT;
        delete[] X;
        delete[] Y;
    }

    void init(Ldbl xmin, Ldbl xmax)
    {
        Xmin = xmin;
        Xmax = xmax;
        Ldbl dPI = PI_2 / (Ldbl)Np;
        for (int j = 0; j < Np; j++)
        {
            Ldbl vj = cosl((2 * j + 1) * dPI);
            X[j] = Xmin + (1.0L + vj) * (Xmax - Xmin) / 2.0L;
        }
    }

    Ldbl Get_V(Ldbl x) { return 2.0L * (x - Xmin) / (Xmax - Xmin) - 1.0L; }
    Ldbl Get_X(int i)  { return X[i]; }
    Ldbl Get_Y(int i)  { return Y[i]; }
    Ldbl Get_aT(int k) { return aT[k]; }
};
