#include <stdio.h>

#include <math.h>

void afisare(unsigned long long harta)
{
	unsigned long long mask=1ull<<63ull;
	int i;
	for(i=1;i<=64;i++)
        {
                
                if((harta&mask)!=0)
			printf("#");
		else
			printf(".");
		//dau sfarsit de linie la capatul liniei
                if(i%8==0)
                printf("\n");
                mask>>=1ull;

        }
	printf("\n");

}

int coliziune(unsigned long long harta,unsigned long long piesa)
{
	if( ( piesa>>8ull & harta ) != 0 )
		return 1;
	else
	       	return 0;
}

unsigned long long wall(int sens)
{
	unsigned long long Wall;
	//daca sens = 1 atunci se
	//genereaza peretele din dreapta
	//daca sens = 0 atunci se
	//genereaza peretele din stanga
	Wall=(257ull)|(257ull<<16ull);
	Wall|=(Wall<<32ull);
	if(sens==0) 
		Wall<<=7ull;
	return Wall;
}

unsigned long long mtr(unsigned long long harta,unsigned long long piesa,int T)
{
	//sensul mutarii este 1 pentru mutare in dreapta
	//si 0 pentru mutare in stanga
	int sens=1,ok=1;
	unsigned long long Wall;
	if( T<0 )
	{
		sens=0;
	}
	//generez in functie 
	//de sensul mutarii peretele
	Wall=wall(sens);
	//se muta piesa pana cand ajung in 
	//dreptul partii ocupate din harta
	//sau la perete
	if(sens==1)
	{
		for(;T && ok==1 ; T--)
		{
			if( ( ( harta & (piesa>>1ull) ) !=0 ) || ( ( Wall & piesa ) !=0 ) )
				ok=0;
			else
				piesa>>=1ull;
		}

	}
	else
	{
		for(;T && ok==1 ; T++)
                {
                	if( ( ( harta & (piesa<<1ull) ) ) !=0 || ( ( Wall & piesa ) !=0 ) )
               			ok=0;
                	else
                    		 piesa<<=1;
                }
	}
	//in final se returneaza 
	//pozitia piesei dupa mutare
	return piesa;

}

int line(unsigned long long harta)
{
	int line=8;
	unsigned long long mask=255ull;
	mask<<=56ull;
	//masca verifica daca o linie este completa
	while(line>0 && ( mask != ( mask & harta) ) )
	{
		mask>>=8ull;
		line--;
	}
	//se returneaza numarul linie 
	//iar daca nici o linie nu e completata se returneaza 0
	return line;
}

unsigned long long rmline(unsigned long long harta,int line)
{
	unsigned long long up,down,mask=255ull;
	while(line>1)
	{
		mask= mask | (mask <<8ull);
		line--;
	}
	//down salveaza partea hartii cu o pozitie mai jos
	//de linia ce trebuie eliminata
	down= harta & (mask>>8ull);
	mask = ~ mask;
	//up salveaza parte hartii cu o pozitie mai sus
	//de linia ce trebuie eliminata
	//apoi o muta cu o pozitie mai in jos
	up= harta & mask;
	up>>=8ull;
	//in final se returneaza harta cu linia l eliminata
	return up|down;

}

int main()
{
	unsigned long long harta,piesa;
	int moves,ok=1,c,j,T,linie,linii=0,zeros=0,oklinie,tip;
	//c e folosita pentru coliziune
	//T reprezinta trasformarea aplicata piesei
	//ok devine 0 cand jocul se termina prin coliziune
	float score=0;
	scanf("%llu",&harta);
	afisare(harta);
	scanf("%d",&moves);
	while(moves>0 && ok==1)
	{
		c=0;
		//initializez coliziune cu 0 (fals)
		moves--;
		scanf("%llu",&piesa);
		if(piesa>255ull)
			tip=2;
		else
			tip=1;
		piesa<<=48ull;
		scanf("%d",&T);
		//duc piesa pe linia 7 
		//pentru a nu elimina partea superioara
		//iar folosind harta mutata 
		//cu o pozitie in jos , prima mutare
		//a piesei se face ca si cum piesa ar fi pe linia 8
		if( ( (piesa<<8) & harta ) !=0  ) 
		{
			//testarea coliziunii 
			//inainte de intrarea piesei pe harta
			//se face cu piesa 
			//mutata cu o poz mai sus
			ok=0;
			c=1;
			afisare( harta | (piesa<<8) );
			//afisez harta normala si piesa cu o poz in sus
		}
		else
		{
			piesa=mtr(harta>>8,piesa,T);
			//mut piesa iar harta fiind 
			//mutata in jos cu o poz , piesa se
			//se va misca pe linia 8
			//afisez harta normal cu piesa cu baza pe linia 8
			afisare( harta |  (piesa<<8) );
		}
		scanf("%d",&T);
		if( coliziune( harta , piesa<<8 ) )
		{
			if(tip==2)
			{
				if(ok==1)
					afisare( harta | piesa );
				ok=0;
			}
			c=1;
			harta|=(piesa<<8);
		}
		else
		{
			//piesa este coborata cu o pozitie fata de piesa<<8
			//deci folosirea lui piesa
			//e echivalenta coborarii
			piesa=mtr(harta,piesa,T);
			afisare( harta | piesa );
		}
		if( coliziune( harta , piesa ) )
		{	
			c=1;
			harta|=piesa;
		}
		for(j=3; j<=8 && c==0 ;j++)
		{
			scanf("%d",&T);
			piesa>>=8; 
			piesa=mtr(harta,piesa,T);
			if( coliziune(harta,piesa) )
			{
				c=1;
				harta|=piesa;
			}
			afisare(harta|piesa);
		}
		//dupa o coliziune citesc transformarile 
		//care nu au fost efectuate
		while(j<=8)
		{            
			j++;
			scanf("%d",&T);
		}
		oklinie=0;
		harta|=piesa;
		linie=line(harta);
		if(linie>0)
		{
			oklinie=1;
		}
		while(linie>0) 
		{
			linii++;
			harta=rmline(harta,linie);
			linie=line(harta);
		}
		if(oklinie==1)
			afisare(harta);
		
	}
	while(harta!=0)
	{
		zeros++;
		harta&=(harta-1);
	}
	zeros=64-zeros;
	score=sqrt(zeros)+pow(1.25,linii);
	printf("GAME OVER!\n");
	printf("Score:%.2f",score);
	return 0;

}
