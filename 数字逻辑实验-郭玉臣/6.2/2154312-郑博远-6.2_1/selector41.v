`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/09/26 00:45:36
// Design Name: 
// Module Name: selector41
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

module selector41(
    input [3:0] iC0, //��λ�����ź� c0
    input [3:0] iC1, //��λ�����ź� c1
    input [3:0] iC2, //��λ�����ź� c2
    input [3:0] iC3, //��λ�����ź� c3
    input iS1, //ѡ���ź� s1
    input iS0, //ѡ���ź� s0
    output reg [3:0] oZ //��λ����ź� z
    );
    always @(*)
    begin
        case({iS1, iS0})
            2'b00: 
            oZ = iC0;
            2'b01: 
            oZ = iC1;
            2'b10: 
            oZ = iC2;
            2'b11: 
            oZ = iC3;
            default;
        endcase
    end
endmodule
