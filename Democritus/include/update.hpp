#ifndef UPDATE_HPP
#define UPDATE_HPP

/**
 * @brief The main glutIdleFunc callback.
 *        Decides whether to do cutting or normal cluster updates.
 */
void idleCallback();

/**
 * @brief Moves the blade, applies repulsion, breaks bonds, etc.
 *        Called if cutting == true.
 */
void updateCutting();

void mouseCallback(int button, int state, int x, int y);

#endif // UPDATE_HPP

