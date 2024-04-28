module play_audio(
    input play, stop, // Send from software
    input clk, rst, 
    input [2:0]soundchoice, // Choose in software
    output reg audio
);
// value = 100MHz / (1/(audiofileLength) * SampleSize)
    wire [11:0] chomp_addr;
    wire [7:0] chomp_data;
    wire chomp_pwm;
    wire [11:0] PortalPlace_addr;
    wire [7:0] PortalPlace_data;
    wire PortalPlace_pwm;
    wire [13:0] PortalTravel_addr;
    wire [7:0] PortalTravel_data;
    wire PortalTravel_pwm;
    wire [13:0] Move_addr;
    wire [7:0] Move_data;
    wire Move_pwm;
    wire [13:0] GameOver_addr;
    wire [7:0] GameOver_data;
    wire GameOver_pwm;

    wire chomp_on, place_on, travel_on, move_on, gameover_on;

    audio_controller #(22706, 2158) control_chomp(clk, rst, play, chomp_addr, chomp_on);
    chomp_rom chomp(clk, chomp_addr, chomp_data);
    PWM pwm_chomp(clk, rst, chomp_data, chomp_pwm);
    
    audio_controller #(20800, 3629) control_PortalPlace(clk, rst, play, PortalPlace_addr, place_on);
    PortalPlace_rom PortalPlace(clk, PortalPlace_addr, PortalPlace_data);
    PWM pwm_PortalPlace(clk, rst, PortalPlace_data, PortalPlace_pwm);
    
    audio_controller #(22607, 8756) control_PortalTravel(clk, rst, play, PortalTravel_addr, travel_on);
    PortalTravel_rom PortalTravel(clk, PortalTravel_addr, PortalTravel_data);
    PWM pwm_PortalTravel(clk, rst, PortalTravel_data, PortalTravel_pwm);
    
    audio_controller #(22520, 4263) control_Move(clk, rst, play, Move_addr, move_on);
    Move_rom Move(clk, Move_addr, Move_data);
    PWM pwm_Move(clk, rst, Move_data, Move_pwm);

    audio_controller #(22653, 14170) control_GameOver(clk, rst, play, GameOver_addr, gameover_on);
    GameOver_rom GameOver(clk, GameOver_addr, GameOver_data);
    PWM pwm_GameOver(clk, rst, GameOver_data, GameOver_pwm);
    
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
                4: audio <= Move_pwm & move_on;
                5: audio <= GameOver_pwm & gameover_on;
                default: audio <= 0;
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
