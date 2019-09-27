#include "simulation.h"
#include "stdlib.h"

#define DATAFILE "LifeSimulation01.xml"

simulation::simulation()
{
	this->tick_speed = 1000;
}

simulation::~simulation()
{

}

void simulation::set_tick_speed(int new_tick_speed)
{
	this->tick_speed = new_tick_speed;
}

void simulation::increase_tick_speed()
{
	switch(this->tick_speed)
	{
		case x1:
			this->set_tick_speed(1000 / x10);
			break;
		case x10:
			this->set_tick_speed(1000 / x50);
			break;
		case x50:
			this->set_tick_speed(1000 / x100);
			break;
		case x100:
			this->set_tick_speed(1000 / x1);
			break;
		default:
			break;
	}
}

void simulation::run_sim()
{
	int iVal;
	int iPlantCount, iGrazerCount, iPredatorCount, iObstacleCount;
	double dVal;
	int xPos, yPos;
	int diameter;
	int energy;
	char genotype[16];
	int height;

	LifeSimDataParser *lsdp = LifeSimDataParser::getInstance();	// Get the singleton
	lsdp->initDataParser(DATAFILE);

    // Call all the simple get functions and test the results
	// World info functions
	int x_size = lsdp->getWorldWidth();

	int y_size = lsdp->getWorldHeight();

	// Plant info functions
	iVal = lsdp->getInitialPlantCount();
	iPlantCount = iVal;

	dVal = lsdp->getPlantGrowthRate();

	iVal = lsdp->getMaxPlantSize();

	iVal = lsdp->getMaxSeedCastDistance();

	iVal = lsdp->getMaxSeedNumber();

	dVal = lsdp->getSeedViability();

	for(int i=0; i< iPlantCount; i++)
	{
		if(lsdp->getPlantData(&xPos, &yPos, &diameter))
		{
			cout << "Plant " << i << " (" << xPos << ", " << yPos << ") diameter = " << diameter << endl;
		}
		else
		{
			cout << "Failed to read data for plant " << i << endl;
		}
	}

	// Grazer info functions
	iVal = lsdp->getInitialGrazerCount();

	iGrazerCount = iVal;

	iVal = lsdp->getGrazerEnergyInputRate();				// Energy input per minute when grazing

	iVal = lsdp->getGrazerEnergyOutputRate();			// Energy output when moving each 5 DU

	iVal = lsdp->getGrazerEnergyToReproduce();			// Energy level needed to reproduce

	dVal = lsdp->getGrazerMaintainSpeedTime();		// Minutes of simulation to maintain max speed

	dVal = lsdp->getGrazerMaxSpeed();						// Max speed in DU per minute

	for(int i=0; i< iGrazerCount; i++)
	{
		if(lsdp->getGrazerData(&xPos, &yPos, &energy))
		{
			cout << "Grazer " << i << " (" << xPos << ", " << yPos << ") energy level = " << energy << endl;
		}
		else
		{
			cout << "Failed to read data for grazer " << i << endl;
		}
	}

	// Predator info functions
	iVal = lsdp->getInitialPredatorCount();

	iPredatorCount = iVal;

	dVal = lsdp->getPredatorMaxSpeedHOD();			// Get max speed for Homozygous Dominant FF

	dVal = lsdp->getPredatorMaxSpeedHED();			// Get max speed for Heterozygous Dominant Ff

	dVal = lsdp->getPredatorMaxSpeedHOR();			// Get max speed for Homozygous Recessive ff

	iVal = lsdp->getPredatorEnergyOutputRate();			// Energy output when moving each 5 DU

	iVal = lsdp->getPredatorEnergyToReproduce();			// Energy level needed to reproduce

	dVal = lsdp->getPredatorMaintainSpeedTime();		// Minutes of simulation to maintain max speed

	iVal = lsdp->getPredatorMaxOffspring();				// Maximum number of offspring when reproducing

	dVal = lsdp->getPredatorGestationPeriod();		// Gestation period in simulation days 

	iVal = lsdp->getPredatorOffspringEnergyLevel();		// Energy level of offspring at birth

	for(int i=0; i< iPredatorCount; i++)
	{
		if(lsdp->getPredatorData(&xPos, &yPos, &energy, genotype))
		{
			cout << "Predator " << i << " (" << xPos << ", " << yPos << ") energy level = " << energy << ", genotype = " << genotype << endl;
		}
		else
		{
			cout << "Failed to read data for predator " << i << endl;
		}
	}

	// Obstacle info data
	iVal = lsdp->getObstacleCount();						// Number of obstacles

	iObstacleCount = iVal;

	for(int i=0; i< iObstacleCount; i++)
	{
		if(lsdp->getObstacleData(&xPos, &yPos, &diameter, &height))
		{
			cout << "Obstacle " << i << " (" << xPos << ", " << yPos << ") diameter = " << diameter << ", height = " << height << endl;
		}
		else
		{
			cout << "Failed to read data for obstacle " << i << endl;
		}
	}

    grid& sim_grid = grid::get_instance(x_size,y_size);
	while(1)
	{
    	for(int x = 0; x < x_size; x++)
		{
			for(int y = 0; y < y_size; y++)
			{
				sim_grid.get_cell_contents(x, y)->act();
			}
		}
		_sleep(this->tick_speed);
	}
    std::cin.get();
}

int main()
{
    simulation* sim = new simulation();
	sim->run_sim();
    return 0;
}