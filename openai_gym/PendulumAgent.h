/*
 * PendulumAgent.h
 *
 *  Created on: Jun 27, 2016
 *      Author: sabeyruw
 */

#ifndef OPENAI_GYM_PENDULUMAGENT_H_
#define OPENAI_GYM_PENDULUMAGENT_H_

#include "RLLibOpenAiGymAgent.h"

// Env
class Pendulum: public OpenAiGymRLProblem
{
  protected:
    // Global variables:
    RLLib::Range<double>* thetaRange;
    RLLib::Range<double>* velocityRange;

    double theta;
    double velocity;

  public:
    Pendulum() :
        OpenAiGymRLProblem(2, 1, 1),  //
        thetaRange(new RLLib::Range<double>(-M_PI, M_PI)), //
        velocityRange(new RLLib::Range<double>(-8.0f, 8.0f)), theta(0), velocity(0)
    {
      // subject to change
      continuousActions->push_back(0, 0.0f);

      observationRanges->push_back(thetaRange);
      observationRanges->push_back(velocityRange);
    }

    virtual ~Pendulum()
    {
      delete thetaRange;
      delete velocityRange;
    }

    void updateTRStep()
    {

      theta = std::atan2(step_tp1->observation_tp1.at(1), step_tp1->observation_tp1.at(0));
      velocity = step_tp1->observation_tp1.at(2);

      //std::cout << "theta: " << (180.0f / M_PI * theta) << " velocity: " << velocity << " reward: "
      //    << step_tp1->reward_tp1 << std::endl;

      output->o_tp1->setEntry(0, thetaRange->toUnit(theta));
      output->o_tp1->setEntry(1, velocityRange->toUnit(velocity));

      output->observation_tp1->setEntry(0, theta);
      output->observation_tp1->setEntry(1, velocity);

    }
};

class PendulumAgent: public RLLibOpenAiGymAgent
{
  private:
    RLLib::Random<double>* random;
    RLLib::Hashing<double>* hashing;
    RLLib::Projector<double>* projector;
    RLLib::StateToStateAction<double>* toStateAction;

    double alpha_v;
    double alpha_u;
    double alpha_r;
    double gamma;
    double lambda;

    RLLib::Trace<double>* critice;
    RLLib::TDLambda<double>* critic;

    RLLib::PolicyDistribution<double>* policyDistribution;
    RLLib::Range<double>* policyRange;
    RLLib::Range<double>* problemRange;
    RLLib::PolicyDistribution<double>* acting;

    RLLib::Trace<double>* actore1;
    RLLib::Trace<double>* actore2;
    RLLib::Traces<double>* actoreTraces;
    RLLib::ActorOnPolicy<double>* actor;

    RLLib::OnPolicyControlLearner<double>* control;
    RLLib::RLAgent<double>* agent;

    RLLib::RLRunner<double>* simulator;

  public:
    PendulumAgent();
    virtual ~PendulumAgent();
    const RLLib::Action<double>* toRLLibStep();
};

#endif /* OPENAI_GYM_PENDULUMAGENT_H_ */
