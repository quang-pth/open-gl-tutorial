# Break out game

## Overview:
 * This is a simple 2D ping pong classical-style game. 
 * Gameplay: player control the paddle to bounce the ball in order to clear all the bricks in a level.
   * Gray bricks are solid bricks and can't be destroyed by the ball.
   * Other bricks are non-solid bricks and can be destroyed by the ball.

## Basic Controls:
 * Press `A`, `D` to move the paddle horizontally.
 * Press `Space` to push the ball if the ball sticks with the paddle.
 
## Levels:
 * There are four levels in the game.
 * User can press `W` or `S` key to select which level they want to play.
   * Level 1:

![image](https://user-images.githubusercontent.com/57148627/190893813-d0518fb7-6f31-496d-9790-4fdb39e9b716.png)

   * Level 2:

![image](https://user-images.githubusercontent.com/57148627/190893828-0f6150e5-6a87-484f-963b-10fe91dfaede.png)
   
   * Level 3:

![image](https://user-images.githubusercontent.com/57148627/190893958-8e1572d2-e23d-46f0-9fec-d42439244c07.png)
  
   * Level 4:
   
![image](https://user-images.githubusercontent.com/57148627/190893962-7394be04-5b60-4d33-9de4-588e0ceed8ad.png)


## Powerups:
 * There are six types of effects in the game. Four positive effects and two negative effections.
   * Positive effects include:
     * Size: increasing size for the paddle.
     * Speed: increasing speed for the ball.
     * Passthrough: allowing the ball to destroy and passthrough non-solid bricks.
     * Sticky: allowing the ball to stick with the user paddle in order for user to control the ball direction.
   * Negative effects include:
     * Chaos: the scene will be spinning around plus an post-processing effect.
     * Confuse: the scene will be flipped in both dimension X and Y plus an post-processing effect.
     
     
  * In game capture:
   
![image](https://user-images.githubusercontent.com/57148627/190893841-b0eb1ccf-8fb0-49e8-a985-c0cb147191e4.png)

  * Chaos effect:

![image](https://user-images.githubusercontent.com/57148627/190893851-75e357e5-46fc-401c-9f42-9810aca48d6c.png)

  * Confuse effect:

![image](https://user-images.githubusercontent.com/57148627/190893898-2459cc00-0acc-4a56-bf09-da2bcdbd8924.png)

## Win game scene:
 * Press `Enter` to restart the game.
 * Press `ESC` to quit.
 
    * I use the Chaos effect here for no particular reason, because i just love this effect :) 
 
![image](https://user-images.githubusercontent.com/57148627/190893948-d70dab04-387f-40a5-9422-22884caab89a.png)


# Technical aspects:
## Programing languages:
 * This game is built in `C++` from scratch (Visual Studio 2019).
 * Graphics API: `OpengGL version 3.3`.
 
## Libraries:
 * `glfw`: creating the window.
 * `glm`: 2d and 3d math library.
 * `glad`: loading OpenGL API.
 * `FreeType`: rendering text.
 * `irrklang`: embeding audio (music and sounds).
