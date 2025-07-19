#pragma once

#include "dattorro_verb.h"

#pragma region INSTANCE

AudioEffectDattorroVerbInstance::AudioEffectDattorroVerbInstance() {
	reverb_left = DattorroVerb_create();
	reverb_right = DattorroVerb_create();
}

AudioEffectDattorroVerbInstance::~AudioEffectDattorroVerbInstance() {
	if (reverb_left) {
		DattorroVerb_delete(reverb_left);
		reverb_left = nullptr; // Set to nullptr after deletion (defensive programming)
	}
	if (reverb_right) {
		DattorroVerb_delete(reverb_right);
		reverb_right = nullptr; // Set to nullptr after deletion (defensive programming)
	}
}

double AudioEffectDattorroVerbInstance::get_pre_delay() const { return pre_delay; }
void AudioEffectDattorroVerbInstance::set_pre_delay(const double p_pre_delay) {
	pre_delay = p_pre_delay;
	DattorroVerb_setPreDelay(reverb_left, pre_delay);
	DattorroVerb_setPreDelay(reverb_right, pre_delay);
}
double AudioEffectDattorroVerbInstance::get_pre_filter() const { return pre_filter; }
void AudioEffectDattorroVerbInstance::set_pre_filter(const double p_pre_filter) {
	pre_filter = p_pre_filter;
	DattorroVerb_setPreFilter(reverb_left, pre_filter);
	DattorroVerb_setPreFilter(reverb_right, pre_filter);
}
double AudioEffectDattorroVerbInstance::get_input_diffusion1() const { return input_diffusion1; }
void AudioEffectDattorroVerbInstance::set_input_diffusion1(const double p_input_diffusion1) {
	input_diffusion1 = p_input_diffusion1;
	DattorroVerb_setInputDiffusion1(reverb_left, input_diffusion1);
	DattorroVerb_setInputDiffusion1(reverb_right, input_diffusion1);
}
double AudioEffectDattorroVerbInstance::get_input_diffusion2() const { return input_diffusion2; }
void AudioEffectDattorroVerbInstance::set_input_diffusion2(const double p_input_diffusion2) {
	input_diffusion2 = p_input_diffusion2;
	DattorroVerb_setInputDiffusion2(reverb_left, input_diffusion2);
	DattorroVerb_setInputDiffusion2(reverb_right, input_diffusion2);
}
double AudioEffectDattorroVerbInstance::get_decay_diffusion() const { return decay_diffusion; }
void AudioEffectDattorroVerbInstance::set_decay_diffusion(const double p_decay_diffusion) {
	decay_diffusion = p_decay_diffusion;
	DattorroVerb_setDecayDiffusion(reverb_left, decay_diffusion);
	DattorroVerb_setDecayDiffusion(reverb_right, decay_diffusion);
}
double AudioEffectDattorroVerbInstance::get_decay() const { return decay; }
void AudioEffectDattorroVerbInstance::set_decay(const double p_decay) {
	decay = p_decay;
	DattorroVerb_setDecay(reverb_left, decay);
	DattorroVerb_setDecay(reverb_right, decay);
}
double AudioEffectDattorroVerbInstance::get_damping() const { return damping; }
void AudioEffectDattorroVerbInstance::set_damping(const double p_damping) {
	damping = p_damping;
	DattorroVerb_setDamping(reverb_left, damping);
	DattorroVerb_setDamping(reverb_right, damping);
}

void AudioEffectDattorroVerbInstance::reset() {
}

void AudioEffectDattorroVerbInstance::_bind_methods() {
}

void AudioEffectDattorroVerbInstance::_process(const void *src_buffer, AudioFrame *dst_buffer, int frame_count) {
	const AudioFrame *p_src_frames = static_cast<const AudioFrame *>(src_buffer);

	for (int i = 0; i < frame_count; i++) {
		// get input
		float input_left = p_src_frames[i].left;
		float input_right = p_src_frames[i].right;

		// process reverb
		DattorroVerb_process(reverb_left, input_left);
		DattorroVerb_process(reverb_right, input_right);

		// get output
		float reverb_output_left = DattorroVerb_getLeft(reverb_left);
		float reverb_output_right = DattorroVerb_getLeft(reverb_right);

		// mix dry and wet signals
		dry_wet = CLAMP(dry_wet, 0.0, 1.0);
		float output_left = (1.0 - dry_wet) * input_left + reverb_output_left * dry_wet;
		float output_right = (1.0 - dry_wet) * input_right + reverb_output_right * dry_wet;

		// return final mix
		dst_buffer[i].left = output_left;
		dst_buffer[i].right = output_right;
	}
}

#pragma endregion

#pragma region WRAPPER

void AudioEffectDattorroVerb::_bind_methods() {
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, FLOAT, gain);

	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, dry_wet)

	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, pre_delay)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, pre_filter)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, input_diffusion1)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, input_diffusion2)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, decay)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, decay_diffusion)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, damping)
}

CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, dry_wet)

// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, pre_delay)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, pre_filter)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, input_diffusion1)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, input_diffusion2)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, decay_diffusion)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, decay)
// CREATE_GETTER_SETTER(AudioEffectDattorroVerb, double, damping)

double AudioEffectDattorroVerb::get_pre_delay() const { return pre_delay; }
void AudioEffectDattorroVerb::set_pre_delay(const double p_pre_delay) {
	pre_delay = p_pre_delay;
	if (instance.is_valid()) {
		instance->set_pre_delay(pre_delay);
	}
}
double AudioEffectDattorroVerb::get_pre_filter() const { return pre_filter; }
void AudioEffectDattorroVerb::set_pre_filter(const double p_pre_filter) {
	pre_filter = p_pre_filter;
	if (instance.is_valid()) {
		instance->set_pre_filter(pre_filter);
	}
}
double AudioEffectDattorroVerb::get_input_diffusion1() const { return input_diffusion1; }
void AudioEffectDattorroVerb::set_input_diffusion1(const double p_input_diffusion1) {
	input_diffusion1 = p_input_diffusion1;
	if (instance.is_valid()) {
		instance->set_input_diffusion1(input_diffusion1);
	}
}
double AudioEffectDattorroVerb::get_input_diffusion2() const { return input_diffusion2; }
void AudioEffectDattorroVerb::set_input_diffusion2(const double p_input_diffusion2) {
	input_diffusion2 = p_input_diffusion2;
	if (instance.is_valid()) {
		instance->set_input_diffusion2(input_diffusion2);
	}
}
double AudioEffectDattorroVerb::get_decay_diffusion() const { return decay_diffusion; }
void AudioEffectDattorroVerb::set_decay_diffusion(const double p_decay_diffusion) {
	decay_diffusion = p_decay_diffusion;
	if (instance.is_valid()) {
		instance->set_decay_diffusion(decay_diffusion);
	}
}
double AudioEffectDattorroVerb::get_decay() const { return decay; }
void AudioEffectDattorroVerb::set_decay(const double p_decay) {
	decay = p_decay;
	if (instance.is_valid()) {
		instance->set_decay(decay);
	}
}
double AudioEffectDattorroVerb::get_damping() const { return damping; }
void AudioEffectDattorroVerb::set_damping(const double p_damping) {
	damping = p_damping;
	if (instance.is_valid()) {
		instance->set_damping(damping);
	}
}

//
//
//
//

float AudioEffectDattorroVerb::get_gain() const { return gain; };
void AudioEffectDattorroVerb::set_gain(const float p_gain) {
	gain = p_gain;
	if (instance.is_valid()) {
		instance->gain = gain;
	}
};

Ref<AudioEffectInstance> AudioEffectDattorroVerb::_instantiate() {
	instance.instantiate(); // create instnce
	set_gain(get_gain()); // refresh the gain
	return instance;
}

#pragma endregion
