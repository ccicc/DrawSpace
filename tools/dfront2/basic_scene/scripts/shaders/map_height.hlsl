


float ComputeVertexHeight( float4 p_vpos, float4 p_uv, float p_plains_amplitude, float p_mountains_amplitude, 
                            float p_offset, float p_uvnoise_seed1, float p_uvnoise_seed2 )
{
    float fbm_mountains_hl = 16.0;
    float fbm_uvnoise_hl = 0.8;

	float res = 0.0;

	double n_vpos_x = ( p_vpos.x / 2.0 ) + 0.5;
	double n_vpos_y = ( p_vpos.y / 2.0 ) + 0.5;
	double n_vpos_z = ( p_vpos.z / 2.0 ) + 0.5;


	double3 f2;
	f2[0] = lerp( -fbm_mountains_hl, fbm_mountains_hl, n_vpos_x );
	f2[1] = lerp( -fbm_mountains_hl, fbm_mountains_hl, n_vpos_y );
	f2[2] = lerp( -fbm_mountains_hl, fbm_mountains_hl, n_vpos_z );

	float fbm_mountains = Fractal_fBm_classic_perlin( f2, 7, 2.15, 0.25, 0.0 );
	if( fbm_mountains < 0.0 )
	{
		fbm_mountains = 0.0;
	}


	double3 f3;
	f3[0] = lerp( -fbm_uvnoise_hl, fbm_uvnoise_hl, n_vpos_x );
	f3[1] = lerp( -fbm_uvnoise_hl, fbm_uvnoise_hl, n_vpos_y );
	f3[2] = lerp( -fbm_uvnoise_hl, fbm_uvnoise_hl, n_vpos_z );

	//float fbm_uvnoise = Fractal_fBm_wombat_perlin( f3, 7, 2.6, 0.5, 1.0, fbm_params6.x, fbm_params6.y );
    float fbm_uvnoise = Fractal_fBm_wombat_perlin( f3, 7, 2.6, 0.5, 1.0, p_uvnoise_seed1, p_uvnoise_seed2 );

    
    /////////////////////////////////////////////////////////////////////////////// 
    float4 uv_mod = 0.0;

    uv_mod.x = p_uv.x + ( fbm_uvnoise * 0.025 );
    uv_mod.y = p_uv.y + ( -fbm_uvnoise * 0.025 );
    
    float4 color = tex2Dlod( TexturePlanetMap, uv_mod );
    ////////////////////////////////////////////////////////////////////////////////////////
    
    //float plain = fbm_params5.w * color.y;
    float plain = p_plains_amplitude * color.y;

    //float mountains = fbm_mountains * fbm_params3.w + 1000.0;
    float mountains = fbm_mountains * p_mountains_amplitude + 1000.0;
    
	float pn = color.z;
	float pn2 = 9.0 * pn;
	float weight = exp( pn2 ) / 1000.0;
	
	double hl = color.y;
	res = lerp( plain, plain + mountains, hl * weight );

	//res += fbm_params.z;
    res += p_offset;
    
    return res;
}


