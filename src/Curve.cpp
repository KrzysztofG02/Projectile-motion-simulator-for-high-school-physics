#include <stdexcept>


using namespace std;


template <std::size_t N>
gs::Curve<N>::Curve(sf::Color const color):
    mPoints{}
{
    this->setAllPointsColor(color);
}

template <std::size_t N>
gs::Curve<N>::~Curve() {}


template <std::size_t N>
void gs::Curve<N>::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(&mPoints[0], N, sf::PrimitiveType::LineStrip, states);
}


template <std::size_t N>
size_t gs::Curve<N>::getPointCount() const
{
    return N;
}

template <std::size_t N>
sf::Vertex const & gs::Curve<N>::operator[](size_t index) const
{
    if(index >= N)
    {
        throw out_of_range("Index out of range.");
    }

    return mPoints[index];
}

template <std::size_t N>
sf::Vertex & gs::Curve<N>::operator[](size_t index)
{
    if(index >= N)
    {
        throw out_of_range("Index out of range.");
    }

    return mPoints[index];
}


template <std::size_t N>
void gs::Curve<N>::setAllPointsColor(sf::Color const color)
{
    for(sf::Vertex &point : mPoints)
    {
        point.color = color;
    }
}
