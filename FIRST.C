/*-----------------БЛОК ВЫХОДА НА НАЧАЛЬНЫЕ УСЛОВИЯ---------------------*/
#ifndef VIRTUAL
if((nr+qp)>NMAX)
	{
	LoopExit("Выбpана слишком большая длина pяда");
	return;
	}
#endif
 _USE_=2;

PassText("Go...");

for(i=0;i<Q_P;i++)
  {
   yi[i]=(yi[Q_P]-Reload_Val)*i/((float)Q_P)+\
         Reload_Val+0.02*sin(20.0*M_PI*i/((float)Q_P));
  }

/*----------- ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА ---------------*/
 if(Ini_Time()) return;

/*-------------- ПЕРЕХОДНЫЙ ПРОЦЕСС ----------------*/

for(i=0;i<=q+2*p+80;i++) y[i]=din_zw(x[i]=yi[i]);


