class Car
{
public:
	//define the car
	int* balls;	
	int* balls_x;
	int* balls_y;
	int* linkslist;
	int** links;
        int nodes;
        int numlinks;
	int iterations;
	int position;
	Car(int);
	void constructCar();
	void deconstructCar();
	int getCarPosition();
	void score(int,int);
	int GetScore();
	Car* breed(Car*);
	Car* mutate();

	~Car()
	{
		delete balls_x;
		delete balls_y;
		for(int i=0; i<nodes; i++)
			delete links[i];
		delete links;
	}
};
