`timescale 1ns / 1ns

module stall(
    input           in_clk,
    input           in_rst,

    input   [4:0]   in_rs_addr,
    input   [4:0]   in_rt_addr,
    input           in_rs_rena,
    input           in_rt_rena,

    input           in_ex_wena,
    input           in_mem_wena,
    input   [4:0]   in_ex_waddr,
	input   [4:0]   in_mem_waddr,

    output  reg     out_stall
    );

    reg stall_ltime;

    always @ (negedge in_clk or posedge in_rst) 
    begin
        if(in_rst) 
        begin
            out_stall   <= 1'b1;
            stall_ltime <= 1'b0;
        end
        else if (stall_ltime == 0) 
        begin
            // ��ˮ��stallʱ�䵽
            if(out_stall) 
            begin
                out_stall <= 1'b0;
            end
            // �������� ����ͻ
            else
            begin
                // ex�׶���id�׶ζ�д��ͻ��stall 2������
                if(in_ex_wena && ((in_rs_rena && (in_ex_waddr == in_rs_addr)) || (in_rt_rena && (in_ex_waddr == in_rt_addr)))) 
                begin
                    stall_ltime <= 1'b1;
                    out_stall   <= 1'b1;
                end
                // ex�׶���mem�׶ζ�д��ͻ��stall 1������
                else if(in_mem_wena && ((in_rs_rena && (in_mem_waddr == in_rs_addr)) || (in_rt_rena && (in_mem_waddr == in_rt_addr)))) 
                begin
                    stall_ltime <= 1'b0;
                    out_stall   <= 1'b1;
                end
            end
        end
        else 
        begin
            stall_ltime = stall_ltime - 1;
        end
	end
endmodule