// ESP8266 Teams Presence - 1-Piece Enclosure (REFINED)
// Designed for: ESP8266 (25.5x34.6mm) + 48mm LED Ring  
// Units: millimeters

/* [Show What] */
show_main = true;
show_cover = true;
show_separated = true;

/* [Main Dimensions] */
outer_diameter = 65;
inner_diameter = 60;
body_height = 50;
dome_height = 18;
total_height = 68;
wall_thickness = 2.5;

/* [ESP8266 Platform] */
esp_width = 26;
esp_length = 35;
esp_platform_height = 18;
esp_clearance = 10;

/* [LED Ring Platform] */
led_outer_dia = 49;
led_platform_height = 42;
led_ring_thickness = 3;

/* [USB-C Port] */
usb_width = 9.5;
usb_height = 4;
usb_from_bottom = 15;

/* [Bottom Cover] */
cover_height = 4;
cover_tolerance = 0.3;

$fn = 120;

// ============================================
// MAIN BODY
// ============================================
module main_body() {
    difference() {
        union() {
            // Cylindrical body
            cylinder(d=outer_diameter, h=body_height);
            
            // Translucent dome
            translate([0, 0, body_height - 2])
                resize([outer_diameter, outer_diameter, dome_height])
                    sphere(d=outer_diameter);
            
            // Bottom rim
            cylinder(d=outer_diameter, h=cover_height + 1);
        }
        
        // Hollow interior
        translate([0, 0, cover_height])
            cylinder(d=inner_diameter, h=body_height);
        
        // Dome interior (thinner walls)
        translate([0, 0, body_height - 2])
            resize([inner_diameter - 2, inner_diameter - 2, dome_height + 2])
                sphere(d=inner_diameter);
        
        // Bottom opening
        translate([0, 0, -0.1])
            cylinder(d=inner_diameter - 1, h=cover_height + 1.2);
        
        // USB-C port
        translate([outer_diameter/2 - wall_thickness - 0.5, -usb_width/2, usb_from_bottom])
            cube([wall_thickness + 1, usb_width, usb_height]);
        
        // Side ventilation
        for(a = [0:60:300]) {
            rotate([0, 0, a])
                translate([outer_diameter/2 - wall_thickness - 0.5, -1.5, 10])
                    cube([wall_thickness + 1, 3, 15]);
        }
        
        // Wire channels
        for(a = [45, 135]) {
            rotate([0, 0, a])
                translate([outer_diameter/2 - 3, -1, cover_height])
                    cube([3, 2, body_height]);
        }
    }
    
    // ESP8266 platform
    translate([0, 0, esp_platform_height]) {
        difference() {
            cylinder(d=inner_diameter - 2, h=2.5);
            cylinder(d=20, h=3); // wire hole
        }
        
        // Mounting posts
        for(x = [-1, 1]) {
            for(y = [-1, 1]) {
                translate([x * (esp_width/2 - 3), y * (esp_length/2 - 3), 0]) {
                    difference() {
                        cylinder(d=5, h=2.5 + esp_clearance);
                        translate([0, 0, 2.5 + esp_clearance - 3])
                            cylinder(d=2.5, h=4);
                    }
                }
            }
        }
    }
    
    // LED ring platform
    translate([0, 0, led_platform_height]) {
        difference() {
            cylinder(d=inner_diameter - 2, h=3);
            translate([0, 0, -0.1])
                cylinder(d=led_outer_dia - 0.5, h=4);
            cylinder(d=28, h=4);
        }
        
        // LED clips
        for(a = [0, 90, 180, 270]) {
            rotate([0, 0, a])
                translate([led_outer_dia/2 - 1.5, -2, 3])
                    cube([1.8, 4, led_ring_thickness]);
        }
    }
}

// ============================================
// BOTTOM COVER
// ============================================
module bottom_cover() {
    difference() {
        union() {
            // Main plate
            cylinder(d=outer_diameter + 1, h=cover_height);
            
            // Snap lip
            translate([0, 0, cover_height - 0.1])
                cylinder(d=inner_diameter - 1 - cover_tolerance, h=1.5);
        }
        
        // Text
        translate([0, 0, cover_height - 1]) {
            linear_extrude(height=1.2)
                text("TEAMS", size=6, halign="center", valign="center",
                     font="Liberation Sans:style=Bold");
        }
        
        // Ventilation holes
        for(a = [0:45:315]) {
            rotate([0, 0, a])
                translate([outer_diameter/3.5, 0, -0.1])
                    cylinder(d=3, h=cover_height + 0.2);
        }
    }
    
    // Feet
    for(a = [45, 135, 225, 315]) {
        rotate([0, 0, a])
            translate([outer_diameter/2 - 8, 0, 0])
                cylinder(d=7, h=1.5);
    }
}

// ============================================
// DISPLAY
// ============================================
if (show_separated) {
    if (show_main) main_body();
    if (show_cover) {
        translate([outer_diameter + 12, 0, 0])
            bottom_cover();
    }
} else {
    // Assembled
    if (show_main) main_body();
    if (show_cover) bottom_cover();
}
