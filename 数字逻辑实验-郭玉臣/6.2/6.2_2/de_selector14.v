`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/09/26 11:02:42
// Design Name: 
// Module Name: de_selector14
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module de_selector14(
    input iC, //�����ź� c
    input iS1, //ѡ���ź� s1 
    input iS0, //ѡ���ź� s0
    output oZ0, //����ź� z0
    output oZ1, //����ź� z1
    output oZ2, //����ź� z2
    output oZ3  //����ź� z3
);
    reg [3:0] oZ;
    assign {oZ0, oZ1, oZ2, oZ3} = oZ;
    
    always @(*)
        begin
            case({iS1, iS0})
                2'b00: 
                oZ = {iC, 3'b111};
                2'b01: 
                oZ = {1'b1, iC, 2'b11};
                2'b10: 
                oZ = {2'b11, iC, 1'b1};
                2'b11: 
                oZ = {3'b111, iC};
                default;
            endcase
        end
    
endmodule
