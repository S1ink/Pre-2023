package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.I2C;


public class Runtime extends TimedRobot {
    
    public static class R2V extends I2C {
        public static final byte addr = 0x08;

        public R2V() { this(I2C.Port.kMXP); }
        public R2V(I2C.Port p) {
            super(p, addr);
        }


    }


}