#include "car.h"
#include "physicsengine.h"
#include "ball.h"
#include <QtGui>

int randint(int a, int b)
{
	return (qrand()%(b-a+1))+a;
}

Car* Car::mutate()
{
	Car* newcar=new Car(nodes);
	return newcar;
}

Car* Car::breed(Car* c)
{
	Car* newcar=new Car(nodes);
	return newcar;
}

Car::Car(int n)
{
	nodes=n;
	balls_x=new int[nodes];
	balls_y=new int[nodes];
	balls=new int[nodes];
	links=new int*[nodes];
	for(int i=0; i<nodes; i++) links[i]=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	for(int i=0; i<nodes; i++)
	{
		balls_x[i]=randint(5,50);
		balls_y[i]=randint(5,50);
	}
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			links[i][j]=0;
			if(i<=j) continue;
			if(randint(1,3)==1)
			{
				links[i][j]=1;
			}
		}
	}
}
void Car::constructCar()
{
	balls=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	for(int i=0; i<nodes; i++)
	{
		balls[i]=makeBall(balls_x[i],balls_y[i]);
	}
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			if(links[i][j])
			{
				linkslist[numlinks++]=makeLink(i,j);
			}
		}
	}
}
void Car::deconstructCar()
{
	for(int i=0; i<numlinks; i++)
		removeLink(linkslist[i]-i);
	for(int i=0; i<nodes; i++)
		removeBall(balls[i]-i);

	delete balls;
	delete linkslist;
}

int Car::getCarPosition()
{
	int sum=0;
	for (int i=0; i<nodes; i++)
		sum+=getBall(i)->position->x;
	return sum / nodes;
}

void Car::score(int itr, int pos)
{
	iterations=itr;
	position=pos;
}
int Car::GetScore()
{
	return position+(2000- iterations);
}
