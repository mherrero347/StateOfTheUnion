import("stdfaust.lib");

total_level = hslider("total_level", 0.5, 0, 1, 0.01) : si.smoo;
lower_level = hslider("lower_level", 0.5, 0, 1, 0.01) : si.smoo;
mid_level = hslider("mid_level", 0.5, 0, 1, 0.01) : si.smoo;
upper_level = hslider("upper_level", 0.5, 0, 1, 0.01) : si.smoo;

sine_waves_lower = os.osc(110) + os.osc(164.81) + os.osc(220);
sine_waves_mid = os.osc(493.88) + os.osc(587.33) + os.osc(659.25) + os.osc(783.99);
sine_waves_upper = os.osc(880) + os.osc(987.77) + os.osc(1046.50);

process_chain = (sine_waves_lower * lower_level) + (sine_waves_mid * mid_level) + (sine_waves_upper * upper_level);

process = hgroup("ambience", process_chain * total_level);