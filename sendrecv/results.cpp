#include "results.h"
#include <cstdio>
#include <unistd.h>

Results::Results(int outerStatisticalIterations_, int numberofpackages_) :
  outerStatisticalIterations(outerStatisticalIterations_),
  numberofpackages(numberofpackages_),
  summe(numberofpackages),
  time(outerStatisticalIterations * numberofpackages),
  package_vector(numberofpackages),
  innerRuntimeIterations_vector(numberofpackages),
  totaldatasent_vector(numberofpackages)
{
//    std::vector<double> summe(numberofpackages);
    
    //std::cout << "n " << numberofpackages << " time.size " << time.size() << std::endl;
    //std::cout << "Sum size" << summe.size() << std::endl;
}

void Results::setvectors(size_t packagesize_tmp,size_t innerRuntimeIterations,size_t totaldatasent, int idx_numberofpackages){
    package_vector.at(idx_numberofpackages)=packagesize_tmp;
    innerRuntimeIterations_vector.at(idx_numberofpackages)=innerRuntimeIterations;
    totaldatasent_vector.at(idx_numberofpackages)=totaldatasent;
}

void Results::settime(int idx_outeriter, int idx_numberofpackages,double time_ ){
    //std::cout<< "m: "<<m<<" z: "<<z<<" time "<<(endtime-starttime)/2<<std::endl;
    int index = idx_outeriter*numberofpackages+idx_numberofpackages;
    time.at(index) = time_;
    
    std::cout << " " << time.at(index) <<" " << std::endl;
    
    summe.at(idx_numberofpackages) += time_;
}

void Results::calculate(){
    for(int z=0;z<numberofpackages;z++){
        double send_mean=summe.at(z)/outerStatisticalIterations;
        //std::cout <<"summe " <<summe.at(z)<<" send_mean "<<send_mean<<" outeriterations "<<outerStatisticalIterations<<std::endl;
        double diff = 0;
        for (int m=0;m<outerStatisticalIterations;m++){
            int index = m*numberofpackages+z;
            diff+= (send_mean - time.at(index))*(send_mean - time.at(index));
        }
        double rate =(totaldatasent_vector.at(z)/send_mean)/1000000;
        //std::cout <<"totaldatasent " <<totaldatasent_vector.at(z)<<" send_mean "<<send_mean<<std::endl;
        double send_vartime = diff/outerStatisticalIterations;
        double send_stdtime = sqrt(send_vartime);
        double send_std=(send_stdtime/send_mean)*rate;
        
        std::cout<<totaldatasent_vector.at(z)<<"\t"<<innerRuntimeIterations_vector.at(z)<<"\t"<<package_vector.at(z) *sizeof(int)<<"\t"<<send_mean<<"\t"<<send_stdtime<<"\t"<<rate<<"\t"<< send_std<<std::endl;
    }

}