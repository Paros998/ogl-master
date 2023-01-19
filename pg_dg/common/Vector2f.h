#ifndef GRAFIKA_VECTOR2F_H
#define GRAFIKA_VECTOR2F_H

class Vector2f {
    float x, y;
public:
    Vector2f( );
    Vector2f(float x, float y);

    float getX( ) const;

    void setX(float x);

    float getY( ) const;

    void setY(float y);
};

#endif //GRAFIKA_VECTOR2F_H