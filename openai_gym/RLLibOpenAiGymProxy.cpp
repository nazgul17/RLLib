/*
 * RLLibOpenAiGymProxy.cpp
 *
 *  Created on: Jun 25, 2016
 *      Author: sabeyruw
 */

#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
//
#include "RLLibOpenAiGymProxy.h"

RLLibOpenAiGymProxy::RLLibOpenAiGymProxy() :
    agent(NULL)
{
}

RLLibOpenAiGymProxy::~RLLibOpenAiGymProxy()
{
  if (!agent)
  {
    delete agent;
  }
}

std::string RLLibOpenAiGymProxy::toRLLib(const std::string& str)
{
  //std::cout << "recv: " << str << std::endl;

  size_t cmdIdx = str.find_first_of("__"); // Look for commands
  if (cmdIdx != std::string::npos)
  {
    if ((cmdIdx = str.find("__I__")) != std::string::npos)
    {
      if (agent)
      {
        delete agent;
        agent = 0;
      }
      agent = RLLibOpenAiGymAgentRegistry::make(str.substr(cmdIdx + 6));
      return agent ? "__A__" : "__?__";
    }
    else
    {
      return "__?__";
    }
  }

  std::stringstream ss(str);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
      std::back_inserter(tokens));

  agent->problem->step_tp1->observation_tp1.clear();
  std::stringstream ssEpisodeState(tokens[tokens.size() - 1]);
  ssEpisodeState >> agent->problem->step_tp1->episode_state_tp1;

  std::stringstream ssEpisodeReward(tokens[tokens.size() - 2]);
  ssEpisodeReward >> agent->problem->step_tp1->reward_tp1;

  agent->problem->step_tp1->observation_tp1.resize(tokens.size() - 2);
  for (size_t i = 0; i < tokens.size() - 2; ++i)
  {
    std::stringstream ssStateVar(tokens[i]);
    ssStateVar >> agent->problem->step_tp1->observation_tp1[i];
  }

  const RLLib::Action<double>* action_tp1 = agent->toRLLibStep();

  // The action_tp1 will be nullptr when the agent exhausted all the time-steps.
  // Then we send an episode end signal to OpenAI Gym to reset the environment.
  std::stringstream ssAction_tp1;
  if (action_tp1)
  {
    ssAction_tp1 << action_tp1->getEntry();
  }
  else
  {
    ssAction_tp1 << "__E__";
  }

  return ssAction_tp1.str();

}

