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

const int GENERATIONS=0;			//how many breeding generations
const int MAXCARS=1000;				//maximum # of cars.  more than this will segfault
const int KILLMAX=20;				//kill all but this many cars
const int INITIAL_POPULATION=30;	//how many cars we start with
const int NUM_BALLS_IN_CAR = 10;

int WIDTH=500,HEIGHT=500;			//screen width and height
QGraphicsScene* thescene;			//window component
WindowView* view;					//window
int timeunit=1000/660;				//when we're actually showing the car, do a frame every this many milliseconds
int SIMULATION_LENGTH = 2000; 		// frame count of simulation before we give up

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

	//DO THIS after implementation
	// use the deconstructor to prevent memory buffer explosion and general bad stuff
	// be careful of seg fault (delete something that is already deleted)
	// delete car
}

void breed()
{ 	// consider every pair of cars, give them a random possibility of breeding,
	// if decide to breed, breed
	// carr[i]-> breed(car[j]);
}

void mutate()
{	//consider every car,ranomd possibility of cloning
	//mutate the clone
	// car[i]->mutate();

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
