#include "DelayLine.h"
#include "IDelayLineConfig.h"

class StereoDelayLineConfig : public IDelayLineConfig {
private:
  const int CHANNEL_LEFT = 0;
  const int CHANNEL_RIGHT = 1;
  DelayLine<float> delayLineLeft;
  DelayLine<float> delayLineRight;

public:
  void init(int delayLineSize) override;
  void clear() override;
  void processBlock(juce::AudioBuffer<float> &buffer) override;
  void setPointModel(int channel, XYPointModel *model) override;
};
