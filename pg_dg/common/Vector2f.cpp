#pragma once
#include "Vector2f.h"

float Vector2f::getX() const{
        return x;
}

void Vector2f::setX(float x) {
    Vector2f::x = x;
}

float Vector2f::getY( ) const {
    return y;
}

void Vector2f::setY(float y) {
    Vector2f::y = y;
}

Vector2f::Vector2f( ) {
   
}

Vector2f::Vector2f(float x, float y){
    setX(x);
    setY(y);
 }