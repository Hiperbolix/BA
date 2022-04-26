#include <time.h>
#include <iostream>
#include <iomanip>  
#include <cmath>
#include <random>
#include <fstream>


#define N 1000000 //Liczba iteracji
#define m_0 2
#define n 2
#define RND true
#define BIN_WIDTH 3

using namespace std;

//generator liczb losowych double
double d_rand(double lower_bound, double upper_bound)
{
	std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lower_bound, upper_bound);
	return dis(gen);
}

int BA[(m_0*(m_0-1) + 2*n*N)]={0}; // Tablica połączeń

int H_BA[m_0 +N]={0}; // Dane do Histogramu (ilości połączeń)
	
int main(void)
{
	/* Input parametrów z konsoli, z poziomu programu lepiej
	int N, m_0, n;
	
	bool RND;
	cout << "podaj parametry sieci:" << endl;
	cout << "poczatkowa ilosc wezlow = ";
	cin >> m_0;
	cout << "liczba krawedzi jakie tworzy nowy wezel w kolenych iteracjach = ";
	cin >> n;
	cout << "ilosc iteracji = ";
	cin >> N;
	cout << endl;
	cout<< "1 - Sieć losowa, 0 - z preferencją";
	cin>>RND;
	*/
	int iter=0;
	int n_curr=(m_0*(m_0-1));
	int u;
	int max=0;

	
	fstream plik1;
	plik1.open("daneBA.txt", ios::out);
	fstream plik2;
	plik2.open("histogramBA.txt", ios::out);
	fstream plik3;
	plik3.open("histogramBA_BIN.txt", ios::out);
	
	for(int i = 0; i<m_0;i++)
	{
		for(int j=0;j<(m_0-1);j++)
		{
			BA[i]=iter+1;
		}
		iter++;
	}
	for(int i = 0; i<N;i++)
	{
		iter++;
		for(int j = 0; j<(n); j++)
		{
			if(RND)
			{
				u = ceil(d_rand(0,iter));
				u--;
				BA[n_curr+j]=u;
				BA[n_curr+n+j]=iter;
			}
			else
			{
				u = ceil(d_rand(0,n_curr));
				u--;
				BA[n_curr+j]=BA[u];
				BA[n_curr+n+j]=iter;
			}
		}
		n_curr+=2*n;
	}
		
	/*
	for(int i =0; i<(m_0*(m_0-1))+2*n*N;i++)
	{
		plik1 << BA[i] << endl;
	}
	*/
	
	cout<<endl<<"Histogram"<<endl;
	
	for(int i =0; i<(m_0 + N); i++) //Pewnie niekonieczne, ale zawsze mogą być jakieś śmieci w pamięci
	{
		H_BA[i]=0;
	}
	
	for(int i =0; i<(m_0*(m_0-1) + 2*n*N); i++) // Wpisanie danych o połączeniach do histogramu
	{
		H_BA[BA[i]-1]++;
	}
	
	max=H_BA[0];
	
	for(int i =1; i<= (m_0 + N); i++)
	{
		if(H_BA[i]>max) max=H_BA[i];
	}
	
	//cout<<endl<<max<<endl;
	
	int HI_BA[max]={0}; // Histogram (rozkład ilości połączeń)
	
	for(int i=0; i<(m_0 + N);i++)
	{
		HI_BA[H_BA[i]-1]++;
	}
	
	for(int i=0; i<(max);i++)
	{
		cout<<(i+1)<<" "<<HI_BA[i]<<endl;
		plik2 << (i+1) << "\t" << HI_BA[i] << endl;
	}
	
	cout<<endl<<"Log Bin:"<<endl;
	//Log Bin
	
	int u_bin;
	int u_bin_temp;
	int l_bin;
	if(RND)
	{
		u=abs(ceil((max+1-m_0)/BIN_WIDTH));
	}
	else
	{
		u=abs(ceil(log2(max)));
	}
	double BIN_HI_BA[u]={0};
	
	u=0;
	
	
	if(RND)
	{
		l_bin=m_0;
		u_bin=l_bin+BIN_WIDTH;
		while(l_bin<=max)
		{
			u_bin_temp=u_bin;
			if(u_bin_temp>(max)) u_bin_temp=max;
			for(int i=l_bin;i<=u_bin_temp;i++)
			{
				BIN_HI_BA[u]+=HI_BA[i-1];
			}
			BIN_HI_BA[u]=BIN_HI_BA[u]/(u_bin+1-l_bin);
			
			cout<<l_bin<<" do "<<u_bin<<"  "<<BIN_HI_BA[u]<<endl;
			plik3 << l_bin<<"\t"<<u_bin<<"\t"<<BIN_HI_BA[u]<<endl;
			
			u++;
			
			l_bin+=BIN_WIDTH;
			u_bin+=BIN_WIDTH;
		}
	}
	else{
		u_bin =2;
		l_bin=2;
		while(l_bin<=max)
		{
			u_bin_temp=u_bin;
			if(u_bin_temp>(max)) u_bin_temp=max;
			for(int i=l_bin;i<=u_bin_temp;i++)
			{
				BIN_HI_BA[u]+=HI_BA[i-1];
			}
			BIN_HI_BA[u]=BIN_HI_BA[u]/(u_bin+1-l_bin);
			
			cout<<l_bin<<" do "<<u_bin<<"  "<<BIN_HI_BA[u]<<endl;
			plik3 << l_bin<<"\t"<<u_bin<<"\t"<<BIN_HI_BA[u]<<endl;
			
			u++;
			
			l_bin=u_bin+1;
			u_bin=2*u_bin;
		}
	}
	
	plik1.close();
	plik2.close();
	plik3.close();
	
	return 0;
}
