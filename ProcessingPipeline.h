#include <vector>
template <typename SampleType> class ISampleProcessor {
public:
  ~ISampleProcessor() = default;
  virtual SampleType processSample(SampleType sample);
};

template <typename SampleType>
class ProcessingPipeline : private ISampleProcessor<SampleType> {
private:
  std::vector<ISampleProcessor<SampleType> *> processors;
  SampleType processSample(SampleType sample) override {
    for (auto *processor : processors)
      sample = processor->processSample(sample);
    return sample;
  }
};
