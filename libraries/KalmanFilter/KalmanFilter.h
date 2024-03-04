#ifndef KALMAN_FILTER
#define KALMAN_FILTER
#include <BasicLinearAlgebra.h>

template<int N>
BLA::Matrix<N, N> createIdentityMatrix()
{
  BLA::Matrix<N, N> out;
  for (int row = 0; row < N; ++row)
  {
    for (int col = 0; col < N; ++col)
    {
      if (row == col)
      {
        out(row, col) = 1.0f;
      }
      else
      {
        out(row, col) = 0.0f;
      }
    }
  }
  return out;
}

template<int N>
class KalmanFilter
{
public:
  KalmanFilter(float dt, float inputStd, float measurentStd, const BLA::Matrix<N, 1>& initialState, const BLA::Matrix<N, N>& initialUncertainty,
               const BLA::Matrix<N, N>& F, const BLA::Matrix<N, 1>& G, const BLA::Matrix<1, N>& H)
    : dt{dt}
    , processUncertainty{G * ~G * inputStd * inputStd}
    , measurentError{measurentStd * measurentStd}
    , state{initialState}
    , uncertainty{initialUncertainty}
    , I{createIdentityMatrix<N>()}
    , F{F}
    , G{G}
    , H{H}
  {}
  BLA::Matrix<N, 1> update(float input, float measure)
  {
    state = F * state + G * input;
    uncertainty = F * uncertainty * ~F + processUncertainty;
    BLA::Matrix<1, 1> L = (H * uncertainty * ~H + measurentError);
    const BLA::Matrix<N, 1> gain = uncertainty * ~H * Invert(L);
    const BLA::Matrix<1, 1> M = {measure};
    state = state + gain * (M - H * state);
    uncertainty = (I - gain * H) * uncertainty;
    return state;
  }
private:
  const float dt;
  const BLA::Matrix<N, N> processUncertainty;
  const float measurentError;
  BLA::Matrix<N, 1> state;
  BLA::Matrix<N, N> uncertainty;
  const BLA::Matrix<N, N> I;
  const BLA::Matrix<N, N> F;
  const BLA::Matrix<N, 1> G;
  const BLA::Matrix<1, N> H;
};

template<>
class KalmanFilter<1>
{
public:
  KalmanFilter(float dt, float inputStd, float measurentStd, float initialState, float initialUncertainty)
    : dt{dt}, processUncertainty{dt * dt * inputStd * inputStd}, measurentError{measurentStd * measurentStd}, state{initialState}, uncertainty{initialUncertainty}
  {}
  float update(float input, float measure)
  {
    state = state + dt * input;
    uncertainty = uncertainty + processUncertainty;
    float gain = uncertainty / (uncertainty + measurentError);
    state = state + gain * (measure - state);
    uncertainty = (1 - gain) * uncertainty;
    return state;
  }
private:
  const float dt;
  const float processUncertainty;
  const float measurentError;
  float state;
  float uncertainty;
};

#endif
