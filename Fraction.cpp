#include <iostream>
#include <cstdint>
#include <cmath>
#include <chrono>

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

Fraction aprFareyRows(float val, float minErr, int maxIter){
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
 


int main(){



/*
    // 1
    Fraction a, b(5), c(1,2), d(-2, 4);
    std::cout << a << '\n' << b << '\n' << c << '\n' << d << '\n'; // 0/1 5/1 1/2 -1/2
    
    // 2
    Fraction e(d), f = b = a;
    std::cout << e << '\n' << f << '\n'; // -1/2 5/1 

    // 3
    Fraction frac1(5, 12), frac2(3, 10); 
    std::cout << frac1 + frac2 << '\n'; // 43/60
    std::cout << frac1 - frac2 << '\n'; // 7/60
    std::cout << frac1 * frac2 << '\n'; // 1/8
    std::cout << frac1 / frac2 << '\n'; // 25/18
    std::cout << -frac1 << '\n';        // -5/12
    
    // 4
    std::cout << ++frac1 << '\n'; // 17/12
    std::cout << frac1 << '\n';   // 17/12
    std::cout << frac2++ << '\n'; // 3/10
    std::cout << frac2 << '\n';   // 13/10

*/

    std::cout << '\n' << '\n';

    // 5
    Fraction frac3 = apr(0.1428, 0.01, 10);
    std::cout << frac3  << '\n';
    Fraction frac4(3.14159, 0.0001, 30);
    std::cout << frac4  << '\n';
    Fraction frac5 = aprFareyRowsFast(3.14159, 0.0001, 30);
    std::cout << frac5 << '\n';

    const size_t size = 1000000;
    static Fraction temp[size];
    auto time_start = std::chrono::steady_clock().now();
    for(size_t i = 0; i < size; i++){
        temp[i] = aprFareyRowsFast(3.14149265358979, 0.00000000000000001, 1000);
    }
    auto time_end = std::chrono::steady_clock().now();
    double diff = std::chrono::duration_cast<std::chrono::nanoseconds> (time_end - time_start).count();
    std::cout << (diff / size) << "ns\n";
     

    
}
