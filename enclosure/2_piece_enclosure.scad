// ESP8266 Teams Presence - 2-Piece Enclosure (REFINED)
// Designed for: ESP8266 (25.5x34.6mm) + 48mm LED Ring
// Units: millimeters

/* [Show What] */
show_base = true;
show_lid = true;
show_separated = true; // false = assembled view

/* [Main Dimensions] */
outer_diameter = 65;
inner_diameter = 60;
base_height = 45;
lid_height = 25;
wall_thickness = 2.5;
base_thickness = 3;

/* [ESP8266 Board] */
esp_width = 26;
esp_length = 35;
esp_height = 10; // clearance height
esp_cavity_depth = 15;

/* [LED Ring] */
led_outer_dia = 49; // 48mm + tolerance
led_ring_height = 3;

/* [USB-C Port] */
usb_width = 9.5;
usb_height = 4;
usb_from_bottom = 10;

/* [Snap Fit] */
snap_height = 3;
snap_depth = 1.5;
tolerance = 0.25;

$fn = 120;

// ============================================
// BASE
// ============================================
module base() {
    difference() {
        // Main body
        union() {
            // Outer shell
            cylinder(d=outer_diameter, h=base_height);
            // Snap rim
            translate([0, 0, base_height - 0.1])
                cylinder(d=outer_diameter + 2, h=snap_height);
        }
        
        // Hollow interior
        translate([0, 0, base_thickness])
            cylinder(d=inner_diameter, h=base_height);
        
        // ESP8266 cavity
        translate([0, 0, base_thickness])
            hull() {
                translate([-esp_width/2, -esp_length/2, 0])
                    cylinder(d=3, h=esp_cavity_depth);
                translate([esp_width/2, -esp_length/2, 0])
                    cylinder(d=3, h=esp_cavity_depth);
                translate([-esp_width/2, esp_length/2, 0])
                    cylinder(d=3, h=esp_cavity_depth);
                translate([esp_width/2, esp_length/2, 0])
                    cylinder(d=3, h=esp_cavity_depth);
            }
        
        // USB-C port
        translate([outer_diameter/2 - wall_thickness - 0.5, -usb_width/2, usb_from_bottom])
            cube([wall_thickness + 1, usb_width, usb_height]);
        
        // Ventilation slots
        for(a = [0:60:300]) {
            rotate([0, 0, a])
                translate([outer_diameter/2 - wall_thickness - 0.5, -1.5, 20])
                    cube([wall_thickness + 1, 3, 15]);
        }
        
        // Wire pass-through
        cylinder(d=10, h=base_height + snap_height + 5);
    }
    
    // ESP mounting posts
    for(x = [-1, 1]) {
        for(y = [-1, 1]) {
            translate([x * (esp_width/2 - 3), y * (esp_length/2 - 3), base_thickness]) {
                difference() {
                    cylinder(d=5, h=esp_cavity_depth - esp_height);
                    translate([0, 0, esp_cavity_depth - esp_height - 3])
                        cylinder(d=2.5, h=4);
                }
            }
        }
    }
    
    // Cable strain relief
    translate([outer_diameter/2 - 8, 0, base_thickness + 2])
        cube([4, 8, 10], center=true);
}

// ============================================
// LID
// ============================================
module lid() {
    difference() {
        union() {
            // Main lid body
            translate([0, 0, 0])
                cylinder(d=outer_diameter - tolerance, h=lid_height);
            
            // Smooth dome top
            translate([0, 0, lid_height - 5])
                resize([outer_diameter - tolerance, outer_diameter - tolerance, 12])
                    sphere(d=outer_diameter);
            
            // Snap groove
            translate([0, 0, -snap_height + 0.1])
                difference() {
                    cylinder(d=outer_diameter + 2 - tolerance*2, h=snap_height);
                    translate([0, 0, -0.1])
                        cylinder(d=inner_diameter + 1, h=snap_height + 0.2);
                }
        }
        
        // Hollow center with thinner walls
        translate([0, 0, 1.5])
            cylinder(d=inner_diameter - 2, h=lid_height);
        
        // LED ring recess
        translate([0, 0, lid_height - led_ring_height]) {
            difference() {
                cylinder(d=led_outer_dia, h=led_ring_height + 10);
                cylinder(d=27, h=led_ring_height + 10);
            }
        }
        
        // Top ventilation
        for(a = [30:60:330]) {
            rotate([0, 0, a])
                translate([outer_diameter/2 - 3, -0.75, 8])
                    cube([3, 1.5, 10]);
        }
    }
    
    // LED clips
    for(a = [0, 90, 180, 270]) {
        rotate([0, 0, a])
            translate([led_outer_dia/2 - 1, -2, lid_height - led_ring_height])
                cube([1.2, 4, led_ring_height + 1]);
    }
}

// ============================================
// DISPLAY
// ============================================
if (show_separated) {
    if (show_base) {
        translate([-outer_diameter/2 - 8, 0, 0])
            base();
    }
    if (show_lid) {
        translate([outer_diameter/2 + 8, 0, lid_height + snap_height])
            rotate([180, 0, 0])
                lid();
    }
} else {
    // Assembled view
    if (show_base) base();
    if (show_lid) {
        translate([0, 0, base_height + snap_height])
            lid();
    }
}
