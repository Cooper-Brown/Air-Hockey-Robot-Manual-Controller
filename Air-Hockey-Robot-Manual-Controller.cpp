// TRAHT-Manual-Controller.cpp : Defines the entry point for the application.
//


#include "TRAHT-Manual-Controller.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "serial/serial.h"
#include <unordered_map>
#include <iostream>

#define PORT_ID "COM4"

using namespace std;

enum class KeyState
{
    Released,
    Pressed
};

bool robot_L = false;
bool robot_R = false;
bool robot_U = false;
bool robot_D = false;

bool last_robot_L = false;
bool last_robot_R = false;
bool last_robot_U = false;
bool last_robot_D = false;

serial::Serial port;
const std::string xLeft = "x.l\n";
const std::string xRight = "x.r\n";
const std::string xStop = "x.s\n";
const std::string yUp = "y.u\n";
const std::string yDown = "y.d\n";
const std::string yStop = "y.s\n";

void sendCommand(const std::string commandCharacters){
    if (!port.isOpen()) {
        std::cout << "Port is not open" << std::endl;
        return;
    }
    port.write(commandCharacters);
    std::cout << "Sent command: " << commandCharacters;
}

void updateRobotState() {
    // ---------------------------- Update the robot with state changes ----------------------------
    // If the x axis needs to move in a different dimension:
    if ((last_robot_L != robot_L) || (last_robot_R != robot_R)) {
        if (!(robot_L xor robot_R)) 
            sendCommand(xStop);
        else if (robot_R)
            sendCommand(xRight);
        else if (robot_L)
            sendCommand(xLeft);
        last_robot_L = robot_L;
        last_robot_R = robot_R;
    }
    // If the y axis needs to move in a different dimension:
    if ((last_robot_U != robot_U) || (last_robot_D != robot_D)) {
        if (!(robot_U xor robot_D))
            sendCommand(yStop);
        else if (robot_U)
            sendCommand(yUp);
        else if (robot_D)
            sendCommand(yDown);
        last_robot_U = robot_U;
        last_robot_D = robot_D;
    }
}

void handleKeyPress(sf::Keyboard::Key key){
    // std::cout << "Key Pressed: " << key << std::endl;
    // Left Key pressed
    if (key == sf::Keyboard::Left)
        robot_L = true;
    // Right key pressed
    else if (key == sf::Keyboard::Right)
        robot_R = true;
    // Up key pressed
    else if (key == sf::Keyboard::Up)
        robot_U = true;
    // Down key pressed
    else if (key == sf::Keyboard::Down)
        robot_D = true;
    updateRobotState();
}

void handleKeyRelease(sf::Keyboard::Key key){
    // std::cout << "Key Released: " << key << std::endl;
    // Left Key released
    if (key == sf::Keyboard::Left)
        robot_L = false;
    // Right key pressed
    else if (key == sf::Keyboard::Right)
        robot_R = false;
    // Up key pressed
    else if (key == sf::Keyboard::Up)
        robot_U = false;
    // Down key pressed
    else if (key == sf::Keyboard::Down)
        robot_D = false;
    updateRobotState();
}

// WILL CALL HANDLE FUNCTIONS ONLY WHEN A KEY IS PRESSED OR RELEASED
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arrow Key State Change Detection");
    sf::Event event;
    std::unordered_map<sf::Keyboard::Key, KeyState> keyStates;

    port.setBaudrate(9600);
    port.setParity(serial::parity_none);
    port.setPort(PORT_ID);
    port.setStopbits(serial::stopbits_one);
    port.open();
    

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                sf::Keyboard::Key key = event.key.code;
                if (keyStates[key] != KeyState::Pressed) {
                    keyStates[key] = KeyState::Pressed;
                    handleKeyPress(key);
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                sf::Keyboard::Key key = event.key.code;
                if (keyStates[key] != KeyState::Released) {
                    keyStates[key] = KeyState::Released;
                    handleKeyRelease(key);
                }
            }
        }

        window.clear(sf::Color::White);
        window.display();
    }

    port.close();
    return 0;
}
