#ifndef TRIANGLE_MODEL_HPP
#define TRIANGLE_MODEL_HPP


class Model {
public:
    virtual ~Model() {};
    virtual void Draw() = 0;
    virtual void Update(float time) = 0;
};


#endif //TRIANGLE_MODEL_HPP
