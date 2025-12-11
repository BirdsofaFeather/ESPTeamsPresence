// ESP8266 Teams Presence - Dome Status Light
// Classic on-air/status light design with prominent diffuser dome
// Units: millimeters

/* [Display Options] */
show_base = true;
show_dome = true;
show_separated = true; // false = assembled view
show_internal = false; // true = show internal components

/* [Dimensions] */
base_diameter = 75;
base_height = 35;
dome_diameter = 65;
dome_height = 35; // Half-sphere

/* [Wall Thickness] */
base_wall = 2.5;
dome_wall = 1.8; // Thinner for better light diffusion

/* [ESP8266] */
esp_width = 26;
esp_length = 35;

/* [LED Ring] */
led_ring_diameter = 48;

/* [USB Port] */
usb_width = 9.5;
usb_height = 4;

$fn = 100;

// ============================================
// BASE UNIT
// ============================================
module base_unit() {
    difference() {
        union() {
            // Main cylinder
            cylinder(d=base_diameter, h=base_height);
            
            // Dome mounting ring
            translate([0, 0, base_height - 0.1])
                cylinder(d=dome_diameter + 4, h=3);
        }
        
        // Hollow interior
        translate([0, 0, 3])
            cylinder(d=base_diameter - base_wall*2, h=base_height);
        
        // Dome snap groove
        translate([0, 0, base_height + 1])
            rotate_extrude()
                translate([dome_diameter/2 + 1, 0, 0])
                    circle(d=3);
        
        // USB-C port cutout
        translate([base_diameter/2 - base_wall - 0.5, -usb_width/2, 12])
            cube([base_wall + 1, usb_width, usb_height]);
        
        // Bottom cable channel
        translate([-5, -base_diameter/2, 0])
            cube([10, 15, 3.5]);
        
        // Ventilation slots
        for(a = [30:60:330]) {
            rotate([0, 0, a])
                translate([base_diameter/2 - base_wall - 0.5, -2, 8])
                    cube([base_wall + 1, 4, 15]);
        }
    }
    
    // Internal LED ring platform
    translate([0, 0, base_height - 8]) {
        difference() {
            cylinder(d=base_diameter - base_wall*2 - 2, h=2);
            // LED ring opening
            cylinder(d=led_ring_diameter + 1, h=3);
            // Center wire hole
            cylinder(d=28, h=3);
        }
        
        // LED positioning clips
        for(a = [0, 90, 180, 270]) {
            rotate([0, 0, a])
                translate([led_ring_diameter/2, -1.5, 0])
                    cube([2, 3, 4]);
        }
    }
    
    // ESP8266 mounting platform
    translate([0, 0, 8]) {
        difference() {
            cylinder(d=base_diameter - base_wall*2 - 2, h=2);
            cylinder(d=20, h=3); // wire hole
        }
        
        // Mounting posts
        for(x = [-1, 1]) {
            for(y = [-1, 1]) {
                translate([x * (esp_width/2 - 2.5), y * (esp_length/2 - 2.5), 0])
                    cylinder(d=4, h=10);
            }
        }
    }
    
    // Base label ring
    translate([0, 0, 0.8]) {
        difference() {
            cylinder(d=base_diameter - 4, h=0.6);
            cylinder(d=base_diameter - 8, h=1);
        }
    }
}

// ============================================
// DIFFUSER DOME
// ============================================
module diffuser_dome() {
    difference() {
        union() {
            // Main hemisphere dome
            sphere(d=dome_diameter);
            
            // Snap-fit ring
            translate([0, 0, -2])
                rotate_extrude()
                    translate([dome_diameter/2 + 0.8, 0, 0])
                        circle(d=2.5);
        }
        
        // Inner hollow (creates thin shell)
        sphere(d=dome_diameter - dome_wall*2);
        
        // Cut bottom half
        translate([0, 0, -dome_diameter/2 - 5])
            cube([dome_diameter, dome_diameter, dome_diameter], center=true);
        
        // Optional: subtle texture for better diffusion
        // (comment out if you want smooth)
        // for(a = [0:15:345]) {
        //     rotate([0, 0, a])
        //         translate([dome_diameter/2 - 0.3, 0, 0])
        //             cylinder(d=0.4, h=dome_height);
        // }
    }
}

// ============================================
// OPTIONAL: INTERNAL COMPONENTS (for reference)
// ============================================
module internal_components() {
    // ESP8266 board
    color("blue", 0.7)
        translate([-esp_width/2, -esp_length/2, 8])
            cube([esp_width, esp_length, 1.6]);
    
    // LED ring
    color("white", 0.8)
        translate([0, 0, base_height - 8])
            difference() {
                cylinder(d=led_ring_diameter, h=2);
                cylinder(d=27, h=3);
            }
    
    // Simulated light glow
    color("yellow", 0.3)
        translate([0, 0, base_height - 6])
            cylinder(d=led_ring_diameter + 10, h=15);
}

// ============================================
// ASSEMBLY
// ============================================
if (show_separated) {
    // Base
    if (show_base) {
        base_unit();
        if (show_internal) {
            internal_components();
        }
    }
    
    // Dome (separated for printing)
    if (show_dome) {
        translate([base_diameter + 15, 0, dome_height])
            rotate([180, 0, 0])
                color("white", 0.7)
                    diffuser_dome();
    }
} else {
    // Assembled view
    if (show_base) {
        base_unit();
        if (show_internal) {
            internal_components();
        }
    }
    if (show_dome) {
        translate([0, 0, base_height + 3])
            color("white", 0.7)
                diffuser_dome();
    }
}

// ============================================
// PRINTING NOTES
// ============================================
// BASE: Print upright as shown
//   Material: Any opaque filament (black/white recommended)
//   Supports: None needed
//   Infill: 20%
//   
// DOME: Print upside down (flat side on bed)
//   Material: Natural/Clear/White translucent PLA
//   Supports: None needed
//   Infill: 10-15% for best light diffusion
//   Layer height: 0.15mm for smooth finish
//   Print slow (30-40mm/s) for clarity
