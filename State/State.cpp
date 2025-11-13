#include <iostream>
#include <memory>

// Forward declaration
class AudioPlayer;

// =======================
// State interface
// =======================
class State
{
public:
    virtual ~State() = default;

    virtual void play(AudioPlayer& player) = 0;
    virtual void pause(AudioPlayer& player) = 0;
    virtual void stop(AudioPlayer& player) = 0;

    virtual const char* name() const = 0; // for printing
};

// =======================
// Concrete States
// =======================
class StoppedState : public State
{
public:
    void play(AudioPlayer& player) override;
    void pause(AudioPlayer& player) override;
    void stop(AudioPlayer& player) override;
    const char* name() const override { return "Stopped"; }
};

class PlayingState : public State
{
public:
    void play(AudioPlayer& player) override;
    void pause(AudioPlayer& player) override;
    void stop(AudioPlayer& player) override;
    const char* name() const override { return "Playing"; }
};

class PausedState : public State
{
public:
    void play(AudioPlayer& player) override;
    void pause(AudioPlayer& player) override;
    void stop(AudioPlayer& player) override;
    const char* name() const override { return "Paused"; }
};

// =======================
// Context: AudioPlayer
// =======================
class AudioPlayer
{
public:
    AudioPlayer()
        : state(std::make_unique<StoppedState>()) // initial state
    {
        std::cout << "[Player] Initial state: " << state->name() << "\n\n";
    }

    // Called by states to change the current state
    void setState(std::unique_ptr<State> newState)
    {
        state = std::move(newState);
        std::cout << "[Player] State changed to: " << state->name() << "\n\n";
    }

    // Public API
    void play()
    {
        std::cout << "Command: play()\n";
        state->play(*this);
        std::cout << "----------------------\n";
    }

    void pause()
    {
        std::cout << "Command: pause()\n";
        state->pause(*this);
        std::cout << "----------------------\n";
    }

    void stop()
    {
        std::cout << "Command: stop()\n";
        state->stop(*this);
        std::cout << "----------------------\n";
    }

private:
    std::unique_ptr<State> state;
};

// =======================
// StoppedState methods
// =======================
void StoppedState::play(AudioPlayer& player)
{
    std::cout << "  [StoppedState] start playing music\n";
    player.setState(std::make_unique<PlayingState>());
}

void StoppedState::pause(AudioPlayer&)
{
    std::cout << "  [StoppedState] pause() has no effect (already stopped)\n";
}

void StoppedState::stop(AudioPlayer&)
{
    std::cout << "  [StoppedState] already stopped\n";
}

// =======================
// PlayingState methods
// =======================
void PlayingState::play(AudioPlayer&)
{
    std::cout << "  [PlayingState] already playing\n";
}

void PlayingState::pause(AudioPlayer& player)
{
    std::cout << "  [PlayingState] pausing music\n";
    player.setState(std::make_unique<PausedState>());
}

void PlayingState::stop(AudioPlayer& player)
{
    std::cout << "  [PlayingState] stopping music\n";
    player.setState(std::make_unique<StoppedState>());
}

// =======================
// PausedState methods
// =======================
void PausedState::play(AudioPlayer& player)
{
    std::cout << "  [PausedState] resume playing\n";
    player.setState(std::make_unique<PlayingState>());
}

void PausedState::pause(AudioPlayer&)
{
    std::cout << "  [PausedState] already paused\n";
}

void PausedState::stop(AudioPlayer& player)
{
    std::cout << "  [PausedState] stopping from paused\n";
    player.setState(std::make_unique<StoppedState>());
}

// =======================
// Demo
// =======================
int main()
{
    AudioPlayer player;

    player.play();   // Stopped -> Playing
    player.pause();  // Playing -> Paused
    player.play();   // Paused  -> Playing
    player.stop();   // Playing -> Stopped
    player.stop();   // already stopped

    return 0;
}
