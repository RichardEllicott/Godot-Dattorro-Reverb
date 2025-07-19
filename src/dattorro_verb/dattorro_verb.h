/*

audio effect template

GDREGISTER_CLASS(AudioEffectDattorroVerbInstance);
GDREGISTER_CLASS(AudioEffectDattorroVerb);

*/
#pragma once

#include "macros.h" // my macros to help declare properties

#include "godot_cpp/classes/audio_effect.hpp"
#include "godot_cpp/classes/audio_effect_instance.hpp"

extern "C" {
#include "thirdparty/dattorro-verb/verb.h"
}

using namespace godot;

// effect instance
class AudioEffectDattorroVerbInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectDattorroVerbInstance, AudioEffectInstance)

private:
	struct sDattorroVerb *reverb_left;
	struct sDattorroVerb *reverb_right;

public:
	AudioEffectDattorroVerbInstance();
	~AudioEffectDattorroVerbInstance() override;

	void reset();

	virtual void _process(const void *src_buffer, AudioFrame *dst_buffer, int frame_count) override;

	float gain = 1.0; // this gain is set by the parent object

	bool stereo = true;
	double dry_wet = 0.2;

	DECLARE_PROPERTY_DEFAULT(double, pre_delay, 0.1)
	DECLARE_PROPERTY_DEFAULT(double, pre_filter, 0.85)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion1, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, damping, 0.95)

protected:
	static void _bind_methods();
};

// the wrapper for the effect
class AudioEffectDattorroVerb : public AudioEffect {
	GDCLASS(AudioEffectDattorroVerb, AudioEffect)

	DECLARE_PROPERTY_DEFAULT(float, gain, 1.0)

	DECLARE_PROPERTY_DEFAULT(bool, stereo, true)

	DECLARE_PROPERTY_DEFAULT(double, dry_wet, 0.2)

	DECLARE_PROPERTY_DEFAULT(double, pre_delay, 0.1)
	DECLARE_PROPERTY_DEFAULT(double, pre_filter, 0.85)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion1, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, damping, 0.95)

protected:
	static void _bind_methods();

public:
	AudioEffectDattorroVerb() = default;

	Ref<AudioEffectDattorroVerbInstance> instance;

	virtual Ref<AudioEffectInstance> _instantiate() override;
};