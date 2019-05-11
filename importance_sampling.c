#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>



#define M_PI 3.14159265358979323846

//int rand(void);
float aleatorio(void);
float random_walk(float x,float delta);
float dist(float x);
float muestreo(float *x, int N, float delta,float *suma);
float integral(float x_ini,int N, float delta);
float delta_optimo(float x_ini,int N, float delta);

int main(int argc,char *argv[]){
	int N;	
	float a;

	srand(time(NULL)); 
	sscanf(argv[1], "%d", & N);

	a=integral(0.5,N, 0.6);
	delta_optimo(0.5, N, 0.6);   

	printf("\nEl valor de la integral es:  %f \n", a);	
	return 0;
}





float random_walk(float x,float delta){                    //me dice el proximo punto en una ventana de delta
	float a;
	a=aleatorio();
	return x+(a-0.5)*delta;	
}	

float aleatorio(void){					//devuelve un numero aeatorio entre 0 y 1
	return ((float)rand())/((float)RAND_MAX);
}


float dist(float x){					//esta es la distriubución a estudiar
	float a;
	a=exp(-(pow(x,2))/2);
	return a;
}






float muestreo(float *x, int N, float delta,float *suma){
	int i;
	float p, nuevo_x,a;
	i=0;
	FILE *fpb= fopen("muestreo", "a");            //Abre un archivo ya creado y vuelca mas información adentro
	int aceptados = 0;			      //para una nueva medición es conveniente borrar el archivo a menos que se quira acumular 								datos (para hacer esto ultimo no hay que cambiar N la longitud del muestro)
	
	*suma=0;
	
	fprintf(fpb, "%f", delta);	
	while(i<N-1){
		nuevo_x=random_walk(*(x+i), delta);

		a=pow(nuevo_x,2)-pow(*(x+i),2);		
		
		if(a<0){				//si a<0 la exponencial es mayor a 1 entonces acepto con probabilidad 1
			*(x+i+1)=nuevo_x;
			aceptados++;
		}
		else{
			p=exp(-a/2)/(2*delta);   
			if(aleatorio()<p){
				*(x+i+1)=nuevo_x;
				aceptados++;		
			}
			else{
				*(x+i+1)=*(x+i);
			}
		}
			

		i++;	
		fprintf(fpb, " %f", nuevo_x);         //para cada delta guarda la totalidad de los pasos (aceptados o no)
	}
	for(i=0;i<N;i++){
		*suma=*suma+pow(*(x+i),2)/N;
	}
	
	fprintf(fpb, "\n");
	*suma=sqrt(2*M_PI)*(*suma);

fclose(fpb);
return ((float)aceptados)/((float)i);
}


float integral(float x_ini,int N, float delta){     //calcula la integral una unica vez
	float *suma, *x;
	
	x=(float*)malloc(N*sizeof(float));
	suma=(float*)malloc(sizeof(float));	

	*x=x_ini;	
	
	muestreo(x, N, delta,suma);

	
free(x);
return *suma;
}


float delta_optimo(float x_ini,int N, float delta){
	float *delt,paso_delta,delta_inicio,delta_fin,*suma,*x;	
	int i,long_delta;	
	
	delta_inicio=0;         //arrancar en 1.4
	delta_fin=10;		//finalizar en 1.5  esto garantiza probabilidad de aceptacion aproximadamente 1/2 (dist gaussiana de ptos).
	long_delta=100;
	paso_delta=((float)(delta_fin-delta_inicio))/((float)long_delta);

	FILE *fdelta= fopen("Histograma_delta", "w");	
	
	x=(float*)malloc(N*sizeof(float));
	delt=(float*)malloc(long_delta*sizeof(float));
	suma=(float*)malloc(sizeof(float));
	
	*delt=delta_inicio;
	for(i=0;i<long_delta;i++){
		*(delt+i)=delta_inicio+paso_delta*i;
	}	
		
	*x=x_ini;	
	
	for(i=0;i<long_delta;i++){					//barre en delta e imprime delta, tasa de aceptación y resultado
		fprintf(fdelta, "%f %f %f \n", *(delt+i), muestreo(x, N,*(delt+i),suma), *suma);
	}
		
free(x);
free(delt);
free(suma);
fclose(fdelta);
return *suma;
}


