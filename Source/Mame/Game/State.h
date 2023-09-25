#pragma once

template<class T>
class State
{
public:
    State(T* owner, const char* name) : owner(owner), name(name) {}
    virtual ~State() {}

    virtual void Initialize() = 0;
    virtual void Update(const float& elapsedTime) = 0;
    virtual void Finalize() = 0;

public:// 取得・設定
    const char* GetName() { return name; }

    // タイマー関連
    void SetTimer(float time) { timer = time; }
    float GetTimer()const { return timer; }
    void SubtractTime(float time) { timer -= time; }

private:
    float timer = 0;    // タイマー

protected:
    T* owner;
    const char* name;
};