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
    
    audio_controller control_chomp(clk, rst, 22706, chomp_addr);
    chomp_rom chomp(clk, chomp_addr, chomp_data);
    PWM pwm_chomp(clk, rst, chomp_data, chomp_pwm);
    
    audio_controller control_PortalPlace(clk, rst, 20800, PortalPlace_addr);
    PortalPlace_rom PortalPlace(clk, PortalPlace_addr, PortalPlace_data);
    PWM pwm_PortalPlace(clk, rst, PortalPlace_data, PortalPlace_pwm);
    
    audio_controller control_PortalTravel(clk, rst, 22607, PortalTravel_addr);
    PortalTravel_rom PortalTravel(clk, PortalTravel_addr, PortalTravel_data);
    PWM pwm_PortalTravel(clk, rst, PortalTravel_data, PortalTravel_pwm);
    
    always @ (posedge clk, posedge rst) begin
        if(rst) begin
            audio <= 0;
        end
        else begin
            if (play) begin
                case (soundchoice)
                    0:  audio <= 0; // Play nothing
                    1:  audio <= chomp_pwm; // PWM modded output
                    2:  audio <= PortalPlace_pwm;
                    3:  audio <= PortalTravel_pwm;
                    default: audio <= 0;
                endcase
            end
            else if (stop) begin
                audio <= 0;
            end
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

module audio_controller(
input clk, rst, 
input [15:0] value, 
output reg [31:0] addr 
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
        count <= count + 1;
        if (count >= value) begin
            addr <= addr + 1;
            count <= 0;
        end
    end
end
endmodule