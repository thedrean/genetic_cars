class Box
{
	int value;
	int weight;
	Box();
};

class DNA
{
	bool boxes[50];

	DNA();
	int getValue();
	int getWeight();
};

Box* box;	//array of boxes

DNA* dna;	//population
int population=0;	//population: how many dnas

Box::Box()
{
	//value random 1 to 100
	//weight random  1 to 50
}

DNA::DNA()
{
	//for i in 50
		//boxes[i] = random true or false
}

int DNA::getWeight()
{
	//sum up the weights & return it
}

int DNA::getValue()
{
	//sum up the values & return it
}

main()
{
	//make my boxes
	//for i in 50, new Box()

	//for initial pop times
	//	dna[i]=new DNA()
	// 	only add it if the weight below 1000


	//for loop # of generations
	//	figure out who the best is and print out its dna/weight/value
	//	kill
	//	breed
	//	mutate

}
