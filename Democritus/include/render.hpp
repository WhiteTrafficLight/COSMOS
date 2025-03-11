#ifndef RENDER_HPP
#define RENDER_HPP

/**
 * @brief GLUT display callback. Renders all objects each frame.
 */
void displayCallback();

/**
 * @brief GLUT reshape callback. Adjusts viewport/projection on window resize.
 */
void reshapeCallback(int w, int h);

#endif // RENDER_HPP

