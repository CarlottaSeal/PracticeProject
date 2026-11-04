#include <chrono>
#include <queue>
//simulation traffic light
//
enum State
{
    -1,
    GREEN,
    RED,
};
struct Light
{
private:
    State state;
    int period;
    
    std::queue<int> indexOfCars;
    
    Light(int period, State state = GREEN)
    {
        
    }
    //void update(std::vector<int> comingcars);
    
    //vector<Light> lights(4);
    //vector<int> carsN;
    //vector<int> carsS;
    //vector<int> carsW;
    //vector<int> carsE;
    std::vector<vector<int>> listOfCars;
    
    for (int i = 0; i < lights.size(); i++)
    {
        light.update(comingCars, i);
    }

    void update(std::vector<int> comingcars, int lightIndex)
    {
        //1.
        if (lights[lightIndex].state == GREEN)
        {
            for (int i =0; i < period; i++)
            {
                if (!light.indexOfCars.empty())
                    light.pop_front();
            }
            
        }
        
        //2. 
        
        pushing into it
        
        
        
        
        //swap
        state = (state + 1) % 2;
    }
};

accum += framerate
lightDuration -= accum;

//10.29 Nvidia onsite 面试小考题 （Robert轮）