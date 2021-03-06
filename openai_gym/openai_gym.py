import gym
import time
import socket
import numpy as np

# This interfaces provide access to RLLib C++ interface via TCP communication. 
class TcpClient(object):
    
    def __init__(self, host, port):
        self.host = host;
        self.port = port;
        self.client_socket = socket.socket()
        self.client_socket.connect((host, port))
        self.buffer_size = 1024
        
    def send_recv(self, msg):
        self.client_socket.send(msg.encode())
        return self.client_socket.recv(self.buffer_size).decode()


class LearnerAgent(object):
    """ Base class that connects to RLLib functionality via
        interprocessor communication
    """
    
    def __init__(self, env_name, discrete_actions, render, host, port):
        self.env_name = env_name
        self.discrete_actions = discrete_actions
        self.render = render 
        self.env = gym.make(env_name)
        self.client_socket = TcpClient(host, port)
        
    def newMsg(self, observations, reward, episode_state):
        msg = ""
        for x in np.nditer(observations):
            msg += str(x)
            msg += " "
        msg += str(reward)
        msg += " "
        msg += str(episode_state)
        return msg
    
    def run(self):
        
        observations = self.env.reset()
        reward = 0
        done = False
        info = None
        send_msg_to_server = True
        # 0 => new epoch starts, 1 episode starts, 2 episode continue, 3 episode ends
        episode_state = 0 
    
        msg = "__I__ " + self.env_name
        # send init command
        print("msg_init: " + msg)
        # return msg
        msg = self.client_socket.send_recv(msg)
        print("msg: " + msg)
        
        if (msg != "__A__"):
            print("Agent is not ready")
            return
        
        print("epoch starts")
    
        episode_state = 1
        t = 0
        episodes_done = 0;  
        
        while True:
            # Viz
            if (self.render == True):
                self.env.render()
            
            if (send_msg_to_server == True):
                msg = self.newMsg(observations, reward, episode_state)
            
                #print("msg: " + msg)
                action_tp1 = self.client_socket.send_recv(msg)
                #print("action_tp1: " + action_tp1)
            
            if (episode_state == 3):
                observations = self.env.reset()
                episode_state = 1
                send_msg_to_server = True
                #print("new episode starts")
                continue
            
            if (action_tp1 != "__E__"):
                if (self.discrete_actions == True):    
                    observations, reward, done, info = self.env.step(int(action_tp1))
                else:
                    observations, reward, done, info = self.env.step(np.array([float(action_tp1)]))
                    
                t = t + 1
            else:
                # RLLib agent has exhaused all the time steps
                done = True
                send_msg_to_server = False
                print("agent exhaused all time-steps") 
                
            if (done == False):
                episode_state = 2
            else:
                # create terminal msg
                episode_state = 3
                episodes_done = episodes_done + 1;
                print("{}: episode finished after {} timesteps".format(episodes_done, t))
                t = 0;
                
            #time.sleep(4)          
                
        self.client_socket.close()
        

class OnPolicyLearnerAgent(LearnerAgent):
    """ On policy controller
    """
    def __init__(self, env_name, discrete_actions, render):
        super(OnPolicyLearnerAgent, self).__init__(env_name, discrete_actions, render, '127.0.0.1', 2345)        

if __name__ == '__main__':
    OnPolicyLearnerAgent('MountainCar-v0', True, True).run()
    #OnPolicyLearnerAgent('Pendulum-v0', False, True).run()
    #OnPolicyLearnerAgent('Acrobot-v0', True, True).run()
    #OnPolicyLearnerAgent('CartPole-v0', True, True).run()
