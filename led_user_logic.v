`timescale 1ns / 1ps
/////////////////////////////////////////////////////////////////
// Module Name: led_user_logic
/////////////////////////////////////////////////////////////////
module led_user_logic(
 input S_AXI_ACLK,
 input S_AXI_ARESETN,
 input slv_reg_wren,
 input [2:0] axi_awaddr,
 input [31:0] S_AXI_WDATA,
 output reg [7:0] LED
 );

 always @( posedge S_AXI_ACLK )
 begin
 if ( S_AXI_ARESETN == 1'b1 )
 LED <= 7'b0;
 else
 if ((slv_reg_wren == 1) && (axi_awaddr == 0))
 LED <= S_AXI_WDATA[7:0];
 end
endmodule
