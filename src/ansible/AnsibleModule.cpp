#include "AnsibleModule.hpp"

AnsibleModule::AnsibleModule()
: LibAVR32Module("ansible")
{
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configButton(KEY1_PARAM, "KEY 1");
    configButton(KEY2_PARAM, "KEY 2");
    configButton(MODE_PARAM, "MODE");
    configInput(IN1_INPUT, "IN 1");
    configInput(IN2_INPUT, "IN 2");
    configOutput(TR1_OUTPUT, "TR 1");
    configOutput(TR2_OUTPUT, "TR 2");
    configOutput(TR3_OUTPUT, "TR 3");
    configOutput(TR4_OUTPUT, "TR 4");
    configOutput(CV1_OUTPUT, "CV 1");
    configOutput(CV2_OUTPUT, "CV 2");
    configOutput(CV3_OUTPUT, "CV 3");
    configOutput(CV4_OUTPUT, "CV 4");
}

void AnsibleModule::processInputs(const ProcessArgs& args)
{
    bool cv1Normal = !inputs[IN1_INPUT].isConnected();
    if (cv1Normal != firmware.getGPIO(B10))
    {
        firmware.setGPIO(B10, cv1Normal);
        firmware.triggerInterrupt(1);
    }
    bool input1 = inputs[IN1_INPUT].getVoltage() > 0;
    if (input1 != firmware.getGPIO(B08))
    {
        firmware.setGPIO(B08, input1);
        firmware.triggerInterrupt(6);
    }
    bool input2 = inputs[IN2_INPUT].getVoltage() > 0;
    if (input2 != firmware.getGPIO(B09))
    {
        firmware.setGPIO(B09, input2);
        firmware.triggerInterrupt(7);
    }
    bool key1Button = params[KEY1_PARAM].getValue() == 0;
    if (key1Button != firmware.getGPIO(B07))
    {
        firmware.setGPIO(B07, key1Button);
    }
    bool key2Button = params[KEY2_PARAM].getValue() == 0;
    if (key2Button != firmware.getGPIO(B06))
    {
        firmware.setGPIO(B06, key2Button);
    }
    bool modeButton = params[MODE_PARAM].getValue() == 0;
    if (modeButton != firmware.getGPIO(NMI))
    {
        firmware.setGPIO(NMI, modeButton);
    }
}

void AnsibleModule::processOutputs(const ProcessArgs& args)
{
    float cv1 = dacToVolts(firmware.getDAC(2));
    float cv2 = dacToVolts(firmware.getDAC(3));
    float cv3 = dacToVolts(firmware.getDAC(0));
    float cv4 = dacToVolts(firmware.getDAC(1));

    bool tr1 = firmware.getGPIO(B02);
    bool tr2 = firmware.getGPIO(B03);
    bool tr3 = firmware.getGPIO(B04);
    bool tr4 = firmware.getGPIO(B05);

    // Update bicolor mode light from B00/B01
    lights[MODE_YELLOW_LIGHT].setSmoothBrightness(firmware.getGPIO(B00) * 0.7, args.sampleTime);
    lights[MODE_WHITE_LIGHT].setSmoothBrightness(firmware.getGPIO(B01) * 0.4, args.sampleTime);

    // Update normal lights
    lights[TR1_LIGHT].setSmoothBrightness(tr1, args.sampleTime);
    lights[TR2_LIGHT].setSmoothBrightness(tr2, args.sampleTime);
    lights[TR3_LIGHT].setSmoothBrightness(tr3, args.sampleTime);
    lights[TR4_LIGHT].setSmoothBrightness(tr4, args.sampleTime);
    lights[CV1_LIGHT].setSmoothBrightness(cv1 / 10.0, args.sampleTime);
    lights[CV2_LIGHT].setSmoothBrightness(cv2 / 10.0, args.sampleTime);
    lights[CV3_LIGHT].setSmoothBrightness(cv3 / 10.0, args.sampleTime);
    lights[CV4_LIGHT].setSmoothBrightness(cv4 / 10.0, args.sampleTime);

    // Update output jacks from GPIO & DAC
    outputs[TR1_OUTPUT].setVoltage(tr1 * 8.0);
    outputs[TR2_OUTPUT].setVoltage(tr2 * 8.0);
    outputs[TR3_OUTPUT].setVoltage(tr3 * 8.0);
    outputs[TR4_OUTPUT].setVoltage(tr4 * 8.0);
    outputs[CV1_OUTPUT].setVoltage(cv1);
    outputs[CV2_OUTPUT].setVoltage(cv2);
    outputs[CV3_OUTPUT].setVoltage(cv3);
    outputs[CV4_OUTPUT].setVoltage(cv4);
}
