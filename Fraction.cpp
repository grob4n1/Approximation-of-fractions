#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <chrono>
#include <vector>

int nod(int64_t a, int64_t b){
        a = abs(a);
        b = abs(b);
        while(a != 0 && b != 0){
            if(a > b){
                a = a % b;
            }
            else{
                b = b % a;
            }
        }
        return a + b;
    }

class Fraction{
public:
    int64_t num;
    uint64_t den;
    Fraction(int64_t knum = 0, uint64_t kden = 1){
        int64_t knod = abs(nod(knum, kden));
        knum = (knum / knod);
        kden = (kden / knod);
        num = knum;
        den = kden;
    }
    Fraction(const Fraction& rv){
        int64_t knod = nod(rv.num, rv.den);
        num = (rv.num / knod);
        den = (rv.den / knod);
    }
    Fraction(float val, float minErr, int maxden){
        if(minErr > 1){
            minErr = 1;
        }
        int aprden = 1;
        int aprnum = 1;
        float nowMinErr = 1000000000;
        for(int i = 2; i <= maxden; i++){
            float nowErr = std::abs(i * val - std::round(i * val));
            if(nowErr < nowMinErr){
                nowMinErr = nowErr;
                aprden = i;
                aprnum = std::round(i * val);
                //std::cout << aprnum << '/' << aprden << '\n';
                if (nowErr <= (minErr)){
                    i = maxden + 1;
                }
            }
        }
        int64_t knod = abs(nod(aprnum, aprden));
        aprnum = (aprnum / knod);
        aprden = (aprden / knod);
        num = aprnum;
        den = aprden;
    }


    Fraction operator= (const Fraction& rv){
        int64_t knod = nod(rv.num, rv.den);
        num = (rv.num / knod);
        den = (rv.den / knod);
        return *this;
    }
    Fraction operator+ (const Fraction& rv) const{
        return Fraction((num * rv.den + rv.num * den), (den * rv.den));
    }
    Fraction operator- (const Fraction& rv) const{
        return Fraction(num * rv.den - rv.num * den, den * rv.den);
    }
    Fraction operator* (const Fraction& rv) const{
        return Fraction(num * rv.num, den * rv.den);
    }
    Fraction operator/ (const Fraction& rv) const{
        return Fraction(num * rv.den, den * rv.num);
    }
    Fraction operator- () const{
        return Fraction(num * (-1), den);
    }
    Fraction operator++ (){
        num = num + den;
        return *this;
    }
    Fraction operator++ (int){
        Fraction temp = *this;
        ++temp;
        return temp;
    }


};
std::ostream& operator<<(std::ostream& stream, const Fraction& frac){
    return stream << frac.num << '/' << frac.den << ' ';
}

std::istream& operator>>(std::istream& stream, Fraction& frac){
    return stream >> frac.num >> frac.den;
}

Fraction apr(float val,float minErr = 0.003, int maxDen = 30){
    if(minErr > 1){
        minErr = 1;
    }
    if(maxDen < 1){
        maxDen = 1;
    }
    uint64_t aprden = 1;
    int64_t aprnum = 1;
    float nowMinErr = 1000000000;
    for(int i = 2; i <= maxDen; i++){
        float nowErr = std::abs(i * val - std::round(i * val));
        //std::cout << std::round(i * val) << '\n';
        if(nowErr < nowMinErr){
            nowMinErr = nowErr;
            aprden = i;
            aprnum = std::round(i * val);
            //std::cout << aprnum << '/' << aprden << '\n';
            if (nowErr <= (minErr)){
                i = maxDen + 1;
            }
        }
    }
    Fraction apr(aprnum, aprden);
    return apr;
}


Fraction aprFareyRowsFast(float val, float minErr, int maxIter){
    if(minErr > 1){
        minErr = 1;
    }
    if(maxIter < 1){
        maxIter = 1;
    }
    double intPart;
    float floatPart;
    floatPart = modf(val, &intPart);
    //std::cout << floatPart << '\n';
    Fraction minVal(0, 1);
    Fraction maxVal(1, 1);
    for(int i = 0; i < maxIter; i++){
        Fraction medianta(minVal.num + maxVal.num, minVal.den + maxVal.den);
        //std::cout << medianta << '\n';
        float distMinVal = floatPart - float(minVal.num) / float(minVal.den);
        float distMaxVal = float(maxVal.num) / float(maxVal.den) - floatPart;
        //std::cout << distMinVal << "  " << distMaxVal << '\n';
        float distMedianta = fabs(floatPart - float(medianta.num) / float(medianta.den));
        if(floatPart < (float(medianta.num) / float(medianta.den))){
            maxVal.num = medianta.num;
            maxVal.den = medianta.den;
        }
        else{
            minVal.num = medianta.num;
            minVal.den = medianta.den;
        }
        if(distMinVal <= minErr){
            i = maxIter;
        }
        if(distMaxVal <= minErr){
            i = maxIter;
        }
    }
    float distMinVal = floatPart - float(minVal.num / minVal.den);
    float distMaxVal = float(maxVal.num / maxVal.den) - floatPart;
    if(distMinVal < distMaxVal){
        minVal.num = int(intPart * minVal.den + minVal.num);
        return minVal;
    }
    else{
        maxVal.num = int(intPart * maxVal.den + maxVal.num);
        return maxVal;
    }
}


class ApproxAlg{
public:
    virtual Fraction approx(float val) = 0;
};


class FractionFarey : public ApproxAlg{
public:
    float val; 
    float minErr; 
    int maxIter;

    FractionFarey(float _val, float _minErr, int _maxIter) : 
    val(_val), minErr(_minErr), maxIter(_maxIter) { }
    
    virtual Fraction approx(float val){
        if(minErr > 1){
        minErr = 1;
    }
    if(maxIter < 1){
        maxIter = 1;
    }
    double intPart;
    float floatPart;
    floatPart = modf(val, &intPart);
    //std::cout << floatPart << '\n';
    Fraction minVal(0, 1);
    Fraction maxVal(1, 1);
    for(int i = 0; i < maxIter; i++){
        Fraction medianta(minVal.num + maxVal.num, minVal.den + maxVal.den);
        //std::cout << medianta << '\n';
        //float distMinVal = floatPart - float(minVal.num / minVal.den);
        //float distMaxVal = float(maxVal.num / maxVal.den) - floatPart;
        //std::cout << distMinVal << "  " << distMaxVal << '\n';
        //float distMedianta = fabs(floatPart - medianta.num / medianta.den);
        if(floatPart < (float(medianta.num) / float(medianta.den))){
            maxVal.num = medianta.num;
            maxVal.den = medianta.den;
        }
        else{
            minVal.num = medianta.num;
            minVal.den = medianta.den;
        }
    }
    float distMinVal = floatPart - float(minVal.num / minVal.den);
    float distMaxVal = float(maxVal.num / maxVal.den) - floatPart;
    if(distMinVal < distMaxVal){
        minVal.num = int(intPart * minVal.den + minVal.num);
        return minVal;
    }
    else{
        maxVal.num = int(intPart * maxVal.den + maxVal.num);
        return maxVal;
    }
    }

};
 
Fraction my_func(size_t idx){
    return Fraction(1, idx);
}

class Test{
private:
    uint64_t testCycles;
    std::vector<float> values;
    std::vector<Fraction> ansewers;
public:
    std::vector<uint64_t> time;
    std::vector<uint64_t> delta_time;
    void generate(size_t size, Fraction (*genFraction)(size_t)){
        values.resize(size);
        ansewers.resize(size);
        time.resize(size);
        delta_time.resize(size - 1);
        for(size_t i = 0; i < size; ++i){
            ansewers[i] = genFraction(i);
            values[i] = (float)ansewers[i].num  / (float)ansewers[i].den;
        }
    }
    void time_test(ApproxAlg* alg){
        for(size_t i = 0; i < values.size(); i++){
            double diff[10];
            for(size_t k = 0; k < 10; k++){
                float valuesi = values[i];
                auto time_start = std::chrono::steady_clock().now();
                for(size_t j = 0; j < i; j++){
                    Fraction temp = alg->approx(valuesi);
                }
                auto time_end = std::chrono::steady_clock().now();
                double diff1 = std::chrono::duration_cast<std::chrono::nanoseconds> (time_end - time_start).count();
                diff[k] = diff1;
                // if(temp != ansewers[i]){
                //     // TODO:
                //     // cout << 
                // }
            }
            double sum_diff;
            for(size_t k = 0; k < 10; k++){
                sum_diff += diff[k];
            }
            time[i] = sum_diff / 10;
        }
        for(size_t i = 0; i < delta_time.size(); i++){
            delta_time[i] = static_cast<float>(time[i + 1]) / static_cast<float>(time[i]);
        }
    }




};


int main(){


    constexpr size_t size = 1000;
    static double time_test[size];
    static Fraction temp[size];

    for(int j = 0; j < size; j++){
        auto time_start = std::chrono::steady_clock().now();
        for(size_t i = 0; i < j; i++){
            temp[i] = apr(3.14149265358979, 0.00000000000000001, 1000);
        }
        auto time_end = std::chrono::steady_clock().now();
        double diff = std::chrono::duration_cast<std::chrono::nanoseconds> (time_end - time_start).count();
        time_test[j] = diff;
    }
    
    //std::cout << (diff / size) << "ns\n";
    //std::cout << time_test[10];
     
    const char* time_apr_path = "time_apr.txt";
    std::ofstream time_apr_file;
    time_apr_file.open(time_apr_path);
    if (time_apr_file.is_open()) {
        for(int i = 0; i < size; i++){
            time_apr_file << time_test[i] << '\n';
        }
        time_apr_file.close();
    }
    

    Test test1;
    test1.generate(size, my_func);
    FractionFarey algFarey(1, 1,1);
    test1.time_test(&algFarey);

    

    const char* time_apprFarey_path = "time_apprFarey.txt";
    std::ofstream time_apprFarey_file;
    time_apprFarey_file.open(time_apprFarey_path);
    if (time_apprFarey_file.is_open()) {
        for(int i = 0; i < size; i++){
            time_apprFarey_file << test1.time[i] << '\n';
        }
        time_apprFarey_file.close();
    }
    for(int i = 0; i < 999; i++){
        if(test1.delta_time[i] > 1.00001){
            std::cout << i << '\n';
        }
    }

}
