module play_audio(
    input play, stop, // Send from software
    input clk, rst, 
    input [1:0]soundchoice, // Choose in software
    output reg audio
);
// value = 100MHz / (1/(audiofileLength) * SampleSize)
    wire [14:0] chomp_addr;
    wire [7:0] chomp_data;
    wire chomp_pwm;
    wire [15:0] PortalPlace_addr;
    wire [7:0] PortalPlace_data;
    wire PortalPlace_pwm;
    wire [16:0] PortalTravel_addr;
    wire [7:0] PortalTravel_data;
    wire PortalTravel_pwm;

    wire chomp_on, place_on, travel_on;

    audio_controller #(22706, 2158) control_chomp(clk, rst, play, chomp_addr, chomp_on);
    chomp_rom chomp(clk, chomp_addr, chomp_data);
    PWM pwm_chomp(clk, rst, chomp_data, chomp_pwm);
    
    audio_controller #(20800, 3629) control_PortalPlace(clk, rst, play, PortalPlace_addr, place_on);
    PortalPlace_rom PortalPlace(clk, PortalPlace_addr, PortalPlace_data);
    PWM pwm_PortalPlace(clk, rst, PortalPlace_data, PortalPlace_pwm);
    
    audio_controller #(22607, 8756) control_PortalTravel(clk, rst, play, PortalTravel_addr, travel_on);
    PortalTravel_rom PortalTravel(clk, PortalTravel_addr, PortalTravel_data);
    PWM pwm_PortalTravel(clk, rst, PortalTravel_data, PortalTravel_pwm);
    
    always @ (posedge clk, posedge rst) begin
        if(rst) begin
            audio <= 0;
        end
        else begin
            case (soundchoice)
                0: audio <= 0; // Play nothing
                1: audio <= chomp_pwm & chomp_on; // PWM modded output
                2: audio <= PortalPlace_pwm & place_on;
                3: audio <= PortalTravel_pwm & travel_on;
            endcase
        end
    end
endmodule

module PWM(
 input clk, rst,
 input [7:0] dutycycle,
 output pwm_out );
 
    reg [7:0] width; // store my duty cycle temporarily
    reg [7:0] count; // counter value that varies from 0 - 255
    always @ (posedge clk, posedge rst) begin
        if (rst) begin
            count <= 0;
            width <= 0;
        end
        else begin
            count <= count + 1;
            if (count == 0) begin
                width <= dutycycle;
            end
            if (width > 0) begin
                width <= width - 1;            
            end 
        end
    end    
    
    assign pwm_out = width ? 1'b1 : 1'b0;
endmodule

module audio_controller #(parameter VALUE=1000, DEPTH=1000) (
input clk, rst, play, 
output reg [31:0] addr, 
output reg count_en 
);
// value = 100MHz / (1/(audiofileLength) * SampleSize)
// chomp is .49 seconds

reg [15:0] count;

always @ (posedge clk, posedge rst) begin
    if (rst) begin
        count <= 0;
        addr <= 0;
    end
    else begin
        if (play) begin 
            count_en <= 1;
            count <= 0;
            addr <= 0;
        end
        else 
            if (count_en && (addr < DEPTH)) begin
                count <= count + 1;
                if (count >= VALUE) begin
                    addr <= addr + 1;
                    count <= 0;
                end
            end
            else begin 
                count <= 0;
                addr <= 0;
                count_en <= 0;
            end
    end
end
endmodule