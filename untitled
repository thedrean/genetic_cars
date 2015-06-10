//GENETIC CARS
//
//Template: Michael Black, 2014
//
//main.cpp does the following in order:
//	1) creates a population of cars
//	2) creates a racecourse
//	3) genetically improves the cars by racing them
//	4) creates the window
//	5) shows the winning cars

#include <iostream>
#include <QtGui>
#include <math.h>
#include "main.h"
#include "physicsengine.h"
#include "car.h"

const int GENERATIONS=30;			//how many breeding generations
const int MAXCARS=1000;				//maximum # of cars.  more than this will segfault
const int KILLMAX=20;				//kill all but this many cars
const int INITIAL_POPULATION=100;	//how many cars we start with
const int NUM_BALLS_IN_CAR = 10;
const int BREEDRATE=20;				//probability of breeding
const int MUTATE_CHANCE=10;		//mutate chance 
const int BALL_MUTATE_RATE=35;			//mutate intensity 
const int LINK_MUTATE_RATE=15;
int WIDTH=500,HEIGHT=500;			//screen width and height
QGraphicsScene* thescene;			//window component
WindowView* view;					//window
int timeunit=1000/660;				//when we're actually showing the car, do a frame every this many milliseconds
int SIMULATION_LENGTH = 2600; 		// frame count of simulation before we give up

Car* car[MAXCARS];					//cars are stored here
int CarCount=INITIAL_POPULATION;	//keeps track of how many valid cars there are
int currentCar;						//which car we're currently racing

int iterations;						//how many frames we have simulated so far

extern bool simulating;				//TRUE if actively racing a car, FALSE during setup
extern bool dontdographics;			//TRUE if no window is open, FALSE if we want to see the cars in the window

//sets up a timer for visualization
TimerHandler::TimerHandler(int t)
{
	QTimer* timer=new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(t);
}

//when a timer goes off, ready to show a frame
//this only happens where we're ready to see the winning cars
void TimerHandler::onTimer()
{	//only shows the winner
	if(!simulating) return;

	doFrame();

	int pos=car[currentCar]->getCarPosition();

	iterations++;

	if(iterations>=SIMULATION_LENGTH || pos>=WIDTH)
	{
		//cut off a car at end of screen or timeout
		qDebug() << iterations<<" iterations, position="<<pos<<endl;
		car[currentCar]->score(iterations,pos);
		car[currentCar]->deconstructCar();

		currentCar++;
		if(currentCar>=CarCount)
		{
			// if all complete, stop racing
			simulating=FALSE;
			for (int i=0; i<CarCount; i++)
			{
				qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations<<", pos: "<<car[i]->position<<endl;
			}
		}
		else
		{
			// if not all done, make next car
			car[currentCar]->constructCar();
		}
		iterations=0;
	}
}

WindowView::WindowView(QGraphicsScene *scene, QWidget* parent):QGraphicsView(scene, parent) { }

void WindowView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton)
	{
		simulating=!simulating; return;
	}
}

void WindowView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		currentCar=0;
		car[currentCar]->constructCar();
		simulating=TRUE;
	}
}

void kill()
{	// sort the cars by scoring mechinism and kill off all but the killmax best
	// 1 car that gets the farthest wins, average ball position?
	// 2 distance > end of track, lowest frame count

	for(int i=0; i<CarCount;i++)
	{
		for(int k=0;k<CarCount-1;k++)
		{
			if(car[k+1]->GetScore()>car[k]->GetScore())
			{
				Car* temp = car[k+1];
				car[k+1]=car[k];
				car[k]=temp;
			}
		}
	}
	qDebug()<<"KILLED"<<endl;
	CarCount=KILLMAX;

	//DO THIS after implementation
	// use the deconstructor to prevent memory buffer explosion and general bad stuff
	// be careful of seg fault (delete something that is already deleted)
	// delete car
}

void breed()
{ 	// consider every pair of cars, give them a random possibility of breeding,
	// if decide to breed, breed
	// carr[i]-> breed(car[j]);
	int breedingpop = CarCount;
	for (int d = 0; d < breedingpop; d++)
	{
		for (int m = 0; m < breedingpop; m++)
		{
			if (m==d)
				continue;
			if (rand()%100<=BREEDRATE)
			{
				CarCount++;
				car[CarCount] = new Car(NUM_BALLS_IN_CAR);
				qDebug()<<"new child"<<endl;
				int crosspoint = rand()%NUM_BALLS_IN_CAR;
	//breeding balls
				for (int i = 0; i < NUM_BALLS_IN_CAR; i++)
				{
					if(i<=crosspoint)
					{
						car[CarCount]->balls_x[i] = car[d]->balls_x[i];
						car[CarCount]->balls_y[i] = car[d]->balls_y[i];
					}
					else
					{
						car[CarCount]->balls_x[i] = car[m]->balls_x[i];
						car[CarCount]->balls_y[i] = car[m]->balls_y[i];
					}
				}
	//breeding links
	//TODO move to car.cpp
				for (int i = 0; i < NUM_BALLS_IN_CAR; i++)
					for (int j = 0; j < NUM_BALLS_IN_CAR; j++)
					{
						if(i<=j)
							continue;
						if(i<=crosspoint)
						{
							car[CarCount]->links[i][j] = car[d]->links[i][j];
						}
						else
						{
							car[CarCount]->links[i][j] = car[m]->links[i][j];
						}


					}


			}
		
			

		}
	}
	qDebug()<<"BREAD"<<endl;
}

void mutate()
{	//consider every car,ranomd possibility of cloning
	//mutate the clone
	// car[i]->mutate();
	int mutatepop = CarCount;
	for (int i = 0; i < mutatepop; i++)
	{

		if(rand()%100<=MUTATE_CHANCE)
		{

			CarCount++;
			car[CarCount] = new Car(NUM_BALLS_IN_CAR);
			qDebug()<<"New Mutant"<<endl;
			//mutate the ballz
			for (int m = 0; m < NUM_BALLS_IN_CAR; m++)
			{
				if (rand()%100<=BALL_MUTATE_RATE)
				{
					car[CarCount]->balls_x[m]= rand()%45 + 5;
					car[CarCount]->balls_y[m]= rand()%45 + 5;
				}
			}
			for (int l = 0; l < NUM_BALLS_IN_CAR; l++)
				for (int k = 0; k < NUM_BALLS_IN_CAR; k++)
				{
					if(l<=k)
						continue;
					if(rand()%100<=LINK_MUTATE_RATE)
						if(car[CarCount]->links[l][k]==0)
							car[CarCount]->links[l][k]=1;
						else
							car[CarCount]->links[l][k]=0;
				}
		}
	}
	qDebug()<<"MUTATED"<<endl;

}

//does all of the racing, breeding etc behind the scenes
//when called, cars are random
//when ended, super cars ahoy, ready to display winners
void doCars()
{
	//no window, don't make visible, happen bts
	dontdographics=TRUE;

	// do it generations times. 
	for(int gen=0; gen<GENERATIONS; gen++) {
		qDebug() << "****** GENERATION "<<gen<<" **********" <<endl;


		for (int i=0; i<CarCount; i++)
		{
			// race every car and get score

			//put car into physics engine
			car[i]->constructCar();

			//run physics engine
			simulating=TRUE;
			int t=0;			//frame count
			int pos;			//car score
			for(t=0; t<SIMULATION_LENGTH; t++) //should probably adjust
			{	//runs 2000 frames
				//move everything 1 epsilon
				doFrame();
				pos=car[i]->getCarPosition();
				if(pos>=WIDTH) break;
			}

			qDebug() << t<<" iterations, position="<<pos<<endl;
			car[i]->score(t,pos);
			simulating=FALSE;
			car[i]->deconstructCar();
		}

		//debug loop
		for (int i=0; i<CarCount; i++)
		{
			qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations<<", pos: "<<car[i]->position<<endl;
		}

		kill();
		breed();
		mutate();
	}

	kill();
	dontdographics=FALSE;
}

extern Wall* walls[];
extern int WallCount;
void makeRaceCourse()
{	//racecourse 
	// should be continuous
	// must trend downhill
	// ditches are bad

	WallCount=9;

	walls[0]=new Wall(1,500,499,500);
	walls[1]=new Wall(-20,132,123,285);
	walls[2]=new Wall(104,285,203,277);
	walls[3]=new Wall(202,275,271,344);
	walls[4]=new Wall(271,344,320,344);
	walls[5]=new Wall(321,345,354,318);
	walls[6]=new Wall(354,318,394,324);
	walls[7]=new Wall(394,324,429,390);
	walls[8]=new Wall(429,391,498,401);

	for(int i=0; i<WallCount; i++)
		thescene->addItem(walls[i]);
}

int main(int argc, char **argv)
{
	//seed random generator
	QApplication app(argc,argv);
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	//make the window
	thescene=new QGraphicsScene();
	thescene->setSceneRect(0,0,WIDTH,HEIGHT);

	//make the course
	makeRaceCourse();

	//make initial population
	for (int i=0; i<CarCount; i++)
		car[i]=new Car(NUM_BALLS_IN_CAR);

	//race cars and breed cars
	doCars();

	//once all cars are bread, show all master cars graphically.
	currentCar=0;
	car[currentCar]->constructCar();
	simulating=TRUE;

	//make thw window
	view=new WindowView(thescene);
	view->setWindowTitle("Genetic Cars");
	view->resize(WIDTH+50,HEIGHT+50);
	view->show();
	view->setMouseTracking(true);

	//start the timer. 
	TimerHandler timer(timeunit);

	return app.exec();
}
