#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo EnvironmentMapEffect.fx /Tps_4_0_level_9_1 /Zpc
//    /Qstrip_reflect /Qstrip_debug /EPSEnvMap
//    /FhCompiled\EnvironmentMapEffect_PSEnvMap.inc
//    /VnEnvironmentMapEffect_PSEnvMap
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// COLOR                    0   xyzw        0     NONE  float   xyzw
// COLOR                    1   xyzw        1     NONE  float   xyzw
// TEXCOORD                 0   xy          2     NONE  float   xy  
// TEXCOORD                 1   xyz         3     NONE  float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_Target                0   xyzw        0   TARGET  float   xyzw
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c0         cb0            11         1  ( FLT, FLT, FLT, FLT)
//
//
// Sampler/Resource to DX9 shader sampler mappings:
//
// Target Sampler Source Sampler  Source Resource
// -------------- --------------- ----------------
// s0             s0              t0               
// s1             s1              t1               
//
//
// Level9 shader bytecode:
//
    ps_2_0
    dcl t0
    dcl t1
    dcl t2.xy
    dcl t3.xyz
    dcl_2d s0
    dcl_cube s1
    texld r0, t3, s1
    texld r1, t2, s0
    mul r1, r1, t0
    mad r0.xyz, r0, r1.w, -r1
    mad r0.xyz, t1, r0, r1
    mad r2.xyz, c0, r1.w, -r0
    mad r1.xyz, t1.w, r2, r0
    mov oC0, r1

// approximately 8 instruction slots used (2 texture, 6 arithmetic)
ps_4_0
dcl_constantbuffer cb0[12], immediateIndexed
dcl_sampler s0, mode_default
dcl_sampler s1, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texturecube (float,float,float,float) t1
dcl_input_ps linear v0.xyzw
dcl_input_ps linear v1.xyzw
dcl_input_ps linear v2.xy
dcl_input_ps linear v3.xyz
dcl_output o0.xyzw
dcl_temps 2
sample r0.xyzw, v3.xyzx, t1.xyzw, s1
sample r1.xyzw, v2.xyxx, t0.xyzw, s0
mul r1.xyzw, r1.xyzw, v0.xyzw
mad r0.xyz, r0.xyzx, r1.wwww, -r1.xyzx
mad r0.xyz, v1.xyzx, r0.xyzx, r1.xyzx
mad r1.xyz, cb0[11].xyzx, r1.wwww, -r0.xyzx
mad o0.xyz, v1.wwww, r1.xyzx, r0.xyzx
mov o0.w, r1.w
ret 
// Approximately 0 instruction slots used
#endif

const BYTE EnvironmentMapEffect_PSEnvMap[] =
{
     68,  88,  66,  67, 153,  99, 
    255,  64, 191, 118,  42, 252, 
     62, 108, 243, 191,  43, 234, 
     97,  92,   1,   0,   0,   0, 
    180,   3,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
     76,   1,   0,   0,   0,   3, 
      0,   0, 128,   3,   0,   0, 
     65, 111, 110,  57,  20,   1, 
      0,   0,  20,   1,   0,   0, 
      0,   2, 255, 255, 220,   0, 
      0,   0,  56,   0,   0,   0, 
      1,   0,  44,   0,   0,   0, 
     56,   0,   0,   0,  56,   0, 
      2,   0,  36,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      1,   1,   1,   0,   0,   0, 
     11,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   2, 
    255, 255,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
     15, 176,  31,   0,   0,   2, 
      0,   0,   0, 128,   1,   0, 
     15, 176,  31,   0,   0,   2, 
      0,   0,   0, 128,   2,   0, 
      3, 176,  31,   0,   0,   2, 
      0,   0,   0, 128,   3,   0, 
      7, 176,  31,   0,   0,   2, 
      0,   0,   0, 144,   0,   8, 
     15, 160,  31,   0,   0,   2, 
      0,   0,   0, 152,   1,   8, 
     15, 160,  66,   0,   0,   3, 
      0,   0,  15, 128,   3,   0, 
    228, 176,   1,   8, 228, 160, 
     66,   0,   0,   3,   1,   0, 
     15, 128,   2,   0, 228, 176, 
      0,   8, 228, 160,   5,   0, 
      0,   3,   1,   0,  15, 128, 
      1,   0, 228, 128,   0,   0, 
    228, 176,   4,   0,   0,   4, 
      0,   0,   7, 128,   0,   0, 
    228, 128,   1,   0, 255, 128, 
      1,   0, 228, 129,   4,   0, 
      0,   4,   0,   0,   7, 128, 
      1,   0, 228, 176,   0,   0, 
    228, 128,   1,   0, 228, 128, 
      4,   0,   0,   4,   2,   0, 
      7, 128,   0,   0, 228, 160, 
      1,   0, 255, 128,   0,   0, 
    228, 129,   4,   0,   0,   4, 
      1,   0,   7, 128,   1,   0, 
    255, 176,   2,   0, 228, 128, 
      0,   0, 228, 128,   1,   0, 
      0,   2,   0,   8,  15, 128, 
      1,   0, 228, 128, 255, 255, 
      0,   0,  83,  72,  68,  82, 
    172,   1,   0,   0,  64,   0, 
      0,   0, 107,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     12,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      1,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  88,  48,   0,   4, 
      0, 112,  16,   0,   1,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   0,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   1,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   2,   0,   0,   0, 
     98,  16,   0,   3, 114,  16, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      3,   0,   0,   0,  70, 126, 
     16,   0,   1,   0,   0,   0, 
      0,  96,  16,   0,   1,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 114,   0,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
     11,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,  32, 
     16,   0,   0,   0,   0,   0, 
    246,  31,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     73,  83,  71,  78, 120,   0, 
      0,   0,   4,   0,   0,   0, 
      8,   0,   0,   0, 104,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0, 104,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,  15,   0,   0, 110,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   3,   0,   0, 110,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   7,   0,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  97, 114, 
    103, 101, 116,   0, 171, 171
};
