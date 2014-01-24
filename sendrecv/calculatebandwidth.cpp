#include "calculatebandwidth.h"
#include <cstdio>
#include <unistd.h>

Calculatebandwidth::Calculatebandwidth(int outerStatisticalIterations_, int numberofpackages_) :
  outerStatisticalIterations(outerStatisticalIterations_),
  numberofpackages(numberofpackages_),
  summe(numberofpackages),
  time(outerStatisticalIterations * numberofpackages)
{
//    std::vector<double> summe(numberofpackages);
    
    //std::cout << "n " << numberofpackages << " time.size " << time.size() << std::endl;
    //std::cout << "Sum size" << summe.size() << std::endl;
}


void Calculatebandwidth::settime(int idx_outeriter, int idx_numberofpackages,double time_ ){
    //std::cout<< "m: "<<m<<" z: "<<z<<" time "<<(endtime-starttime)/2<<std::endl;
    int index = idx_outeriter*numberofpackages+idx_numberofpackages;
    time.at(index) = time_;
    
    //std::cout<< __LINE__ << " " << time.at(index) <<" " << std::endl;
    
    summe.at(idx_numberofpackages) += time_;
    //std::cout<<"hier ist alles klar"<<std::endl;

}

void Calculatebandwidth::calculate(std::vector<size_t> &totaldatasent_vector,std::vector<size_t>&package_vector,std::vector<size_t>&innerRuntimeIterations_vector,std::vector<double> &loadavg_vector){
    sleep(10);
    for(int z=0;z<numberofpackages;z++){
        double send_mean=summe.at(z)/outerStatisticalIterations;
        //cout<<"summe["<<z<<"]: "<<summe[z]<<endl;
        //cout<<"send_mean "<<send_mean<<endl;
        double diff = 0;
        for (int m=0;m<outerStatisticalIterations;m++){
            int index = m*numberofpackages+z;
            diff+= (send_mean - time.at(index))*(send_mean - time.at(index));
        }
        double rate =(totaldatasent_vector.at(z)/send_mean)/1000000;
        //cout << totaldatasent_vector[z]<< endl;
        double send_vartime = diff/outerStatisticalIterations;
        double send_stdtime = sqrt(send_vartime);
        double send_std=(send_stdtime/send_mean)*rate;
        
        printf("%ld %ld %ld %f %f %f %f - %f \n",totaldatasent_vector.at(z) ,innerRuntimeIterations_vector.at(z), package_vector.at(z) *sizeof(int),send_mean, send_stdtime, rate, send_std, loadavg_vector.at(z));
        fflush(stdout);
    }

}