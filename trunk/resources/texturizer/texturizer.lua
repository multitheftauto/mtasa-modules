--[[

	This file is part of the Texturizer module by jyrno42. It is necessary to be loaded since it monitors script
	unloading and cleans up the images which haven't been unloaded yet. It also contains tests that can be done to test
	the module for bugs.

]]--

-- THE CONFIG

--  To enable tests set this to true.
local TESTS = false;
local DEEP_OUTPUT = false;

-- THE SCRIPT

if( not gd_info ) then

	outputDebugString( "Texturizer module is not loaded!", 1, 255, 0, 0 );

else

	-- The automatic cleanup for scripts.
	addEventHandler( "onResourceStop", getRootElement( ),

		function ( theResource )
		
			imageCleanup( theResource );
		
		end

	);

	-- The tests.
	if( TESTS ) then

		function Test_gd_Info( )
		
			if( gd_info ) then
			
				local ret = gd_info( );
				if( ret ) then
				
					if( DEEP_OUTPUT ) then
					
						for k, v in pairs( ret ) do
						
							outputDebugString( "\t\t" .. k .. " => " .. v );
						
						end
					
					end
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageCreate( )
		
			if( imageCreate ) then
			
				local ret = imageCreate( 4, 4 );
				if( ret ) then
				
					imageDestroy( ret );					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageCreateTrueColor( )
		
			if( imageCreateTrueColor ) then
			
				local ret = imageCreateTrueColor( 4, 4 );
				if( ret ) then
				
					imageDestroy( ret );					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageCreateFromPng( )
		
			if( imageCreateFromPng ) then
			
				local ret = imageCreateFromPng( "input.png" );
				if( ret ) then
				
					imageDestroy( ret );					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageCreateFromGif( )
		
			if( imageCreateFromGif ) then
			
				local ret = imageCreateFromGif( "input.gif" );
				if( ret ) then
				
					imageDestroy( ret );					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageCreateFromJpeg( )
		
			if( imageCreateFromJpeg ) then
			
				local ret = imageCreateFromJpeg( "input.jpg" );
				if( ret ) then
				
					imageDestroy( ret );					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageSXY( )
		
			if( imageSX and imageSY ) then
			
				local ret = imageCreateFromJpeg( "input.jpg" );
				if( ret ) then
				
					local w = imageSX( ret );
					local h = imageSY( ret );
					if( DEEP_OUTPUT ) then
					
						outputDebugString( "\t\twidth => " .. tostring( w ) );
						outputDebugString( "\t\theight => " .. tostring( h ) );
					
					end
					
					imageDestroy( ret );					
					
					if( w and h ) then return true; end
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageIsTrueColor( )
		
			if( imageIsTrueColor ) then
			
				local ret = imageCreate( 4, 4 );
				local ret2 = imageCreateTrueColor( 4, 4 );
				if( ret and ret2 ) then
				
					local res1 = imageIsTrueColor( ret ); -- should be false
					local res2 = imageIsTrueColor( ret2 ); -- should be true
					
					if( DEEP_OUTPUT ) then
					
						outputDebugString( "\t\tres1 => " .. tostring( res1 ) );
						outputDebugString( "\t\tres2 => " .. tostring( res2 ) );
					
					end
					
					imageDestroy( ret );					
					imageDestroy( ret2 );					
					
					if( not res1 and res2 ) then return true; end
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageSave( )
		
			if( imagePng and imageGif and imageJpeg ) then
			
				local ret = imageCreateTrueColor( 128, 128 );
				if( ret ) then
				
					local result = imagePng( ret, "tests/Black_1.png" ) and imageGif( ret, "tests/Black_1.gif" ) and imageJpeg( ret, "tests/Black_1.jpg", 1, true );
					local test = imagePng( ret, "tests/Black_2_I_SHOULD_NOT_EXIST.png" ); -- Shouldn't do anything since we deleted the image resource in the last call to imageJpeg.
					
					imageDestroy( ret );
					
					if( result and not test ) then return true; end
				
				end
			
			end
			return false;
		
		end	
		
		function Test_imageAlphaBlending( )
		
			if( imageGetAlphaBlending and imageSetAlphaBlending ) then
			
				local ret = imageCreateTrueColor( 128, 128 );
				if( ret ) then		
				
					local first = imageGetAlphaBlending( ret );
					imageSetAlphaBlending( ret, not first );
					local new = imageGetAlphaBlending( ret );
					
					imageDestroy( ret );
					
					if( new ~= first ) then return true; end
					
					return true;
				
				end
			
			end
			return false;
		
		end	
		
		function Test_imageAntiAlias( )
		
			if( imageGetAntiAlias and imageSetAntiAlias ) then
			
				local ret = imageCreateTrueColor( 128, 128 );
				if( ret ) then		
				
					local first = imageGetAntiAlias( ret );
					imageSetAntiAlias( ret, not first );
					local new = imageGetAntiAlias( ret );
					
					imageDestroy( ret );
					
					if( new ~= first ) then return true; end
					
					return true;
				
				end
			
			end
			return false;
		
		end	
		
		function Test_imageInterlace( )
		
			if( imageGetInterlace and imageSetInterlace ) then
			
				local ret = imageCreateTrueColor( 128, 128 );
				if( ret ) then		
				
					local first = imageGetInterlace( ret );
					imageSetInterlace( ret, not first );
					local new = imageGetInterlace( ret );
					
					imageDestroy( ret );
					
					if( new ~= first ) then return true; end
					
					return true;
				
				end
			
			end
			return false;
		
		end	
		
		function Test_imageSaveAlpha( )
		
			if( imageGetSaveAlpha and imageSetSaveAlpha ) then
			
				local ret = imageCreateTrueColor( 128, 128 );
				if( ret ) then		
				
					local first = imageGetSaveAlpha( ret );
					imageSetSaveAlpha( ret, not first );
					local new = imageGetSaveAlpha( ret );
					
					imageDestroy( ret );
					
					if( new ~= first ) then return true; end
					
					return true;
				
				end
			
			end
			return false;
		
		end	
		
		function Test_imagePaletteColor( )
		
			if( imageColorAllocate ) then
			
				local ret = imageCreate( 128, 128 );
				if( ret ) then		
				
					local red = imageColorAllocate( ret, 255, 0, 0 );
					local green = imageColorAllocateAlpha( ret, 0, 255, 0, 40 );
					
					local red2 = imageColorClosest( ret, 240, 0, 0 );
					local green2 = imageColorClosestAlpha( ret, 0, 240, 0, 40 );
					
					local red3 = imageColorExact( ret, 240, 0, 0 );
					local green3 = imageColorExactAlpha( ret, 0, 255, 0, 40 );
					
					local red4 = imageColorResolve( ret, 255, 0, 0 );
					local green4 = imageColorResolve( ret, 0, 255, 0, 40 );
					
					local colors = imageColorsTotal( ret ); -- should be two since we allocated only two
					
					local blue = imageColorResolve( ret, 0, 0, 255 ); 
					local colors2 = imageColorsTotal( ret ); -- should now be three
					
					imageColorDeallocate( ret, blue );
					local colors3 = imageColorsTotal( ret ); -- should now again be two
					
					local top_left = imageGetColorAt( ret, 0, 0 ); -- should be red...
					
					local w = imageSX( ret )-1;
					local h = imageSY( ret )-1;
					for x=0,w,3 do

						for y=0,h, 1 do
						
							imageSetColorAt( ret, x, y, green );
						
						end
					
					end
					
					if( red and green and red == red2 and green == green2 and red == red4 and green == green4 and not red3 and green3 and colors == 2 and colors2 == 3 and colors3 == 2 and top_left == red ) then
					
						imageGif( ret, "tests/redBackgroundGreenLines.gif" );
						
						return true;
					
					end
					
					imageDestroy( ret );
					
					if( red and lGreen ) then return true; end
					
					return true;
				
				end
			
			end
			return false;
		
		end
		
		function Test_imageColorSpecial( )
		
			if( imageColorSpecial ) then
			
				local tbl = { "styled", "brushed", "styledbrushed", "tiled", "transparent", "antialiased" };
				for k, v in ipairs( tbl ) do
				
					if( not imageColorSpecial( v ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageColorSpecial failed with: " .. tostring( v ) );
						
						end
						return false;
					
					end
					
					return true;
				
				end
			
			end
			
			return false;
		
		end
		
		function Test_imageFont( )
		
			if( imageFontHeight and imageFontWidth ) then
			
				for i=0,5,1 do
				
					if( not imageFontHeight( i ) or not imageFontWidth( i ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageFontHeight failed with: " .. tostring( i ) );
						
						end					
						return false;
					
					end
				
				end
				
				return true;
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageFontHeight ) then outputDebugString( "*** function imageFontHeight doesn't exist!" ); end
				if( not imageFontWidth ) then outputDebugString( "*** function imageFontWidth doesn't exist!" ); end
			
			end
			
			return false;
		
		end	
		
		function Test_imageChar( )
		
			if( imageChar and imageCharUp ) then
			
				for i=0,5,1 do
				
					local ret = imageCreate( 128, 128 );
					local black = imageColorAllocate( ret, 0, 0, 0 );
					if( not ret ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageCreate failed!" );
						
						end
						return false;
					
					end
					
					local green = imageColorAllocate( ret, 0, 255, 0 );
					
					if( not imageChar( ret, i, 0, 0, tostring( i ), green ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageChar failed with font" .. i .. "!" );
						
						end
						return false;
					
					end
					if( not imageCharUp( ret, i, 25, 25, tostring( i ), green ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageCharUp failed with font" .. i .. "!" );
						
						end
						return false;
					
					end
					
					if( not imageGif( ret, "tests/char_" .. i .. ".gif", true ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageGif failed!" );
						
						end
						return false;
					
					end
				
				end
				
				return true;
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageChar ) then outputDebugString( "*** function imageChar doesn't exist!" ); end
				if( not imageCharUp ) then outputDebugString( "*** function imageCharUp doesn't exist!" ); end
			
			end
			
			return false;
		
		end	
		
		function Test_imageString( )
		
			if( imageString and imageStringUp ) then
			
				for i=0,5,1 do
				
					local ret = imageCreate( 128, 128 );
					local black = imageColorAllocate( ret, 0, 0, 0 );
					if( not ret ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageCreate failed!" );
						
						end
						return false;
					
					end
					
					local green = imageColorAllocate( ret, 0, 255, 0 );
					
					if( not imageString( ret, i, 0, 0, "Test" .. tostring( i ), green ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageString failed with font" .. i .. "!" );
						
						end
						return false;
					
					end
					if( not imageStringUp( ret, i, 25, 25, "Test" .. tostring( i ), green ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageStringUp failed with font" .. i .. "!" );
						
						end
						return false;
					
					end
					
					if( not imageGif( ret, "tests/string_" .. i .. ".gif", true ) ) then
					
						if( DEEP_OUTPUT ) then
						
							outputDebugString( "*** imageGif failed!" );
						
						end
						return false;
					
					end
				
				end
				
				return true;
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageString ) then outputDebugString( "*** function imageString doesn't exist!" ); end
				if( not imageStringUp ) then outputDebugString( "*** function imageStringUp doesn't exist!" ); end
			
			end
			
			return false;
		
		end	
		
		function Test_imageTTF( )
		
			if( imageTtfText and imageTtfBBox ) then
			
				local ret = imageCreate( 128, 128 );
				if( ret ) then	
				
					imageColorAllocate( ret, 0, 0, 0 );
					local green = imageColorAllocate( ret, 0, 255, 0 );
					
					local box = imageTtfBBox( 16, 0, "arial.ttf", "CENTER" );
					if( box ) then
					
						local x = (128 - box[3]) / 2;
						local txt = imageTtfText( ret, 16, 0, x, 64, green, "arial.ttf", "CENTER" );
						
						if( txt ) then
						
							imageGif( ret, "tests/centered.gif", true );
							return true;
						
						else
						
							if( DEEP_OUTPUT ) then outputDebugString( "*** function imageTtfText failed!" ); end
						
						end
					
					else
					
						if( DEEP_OUTPUT ) then outputDebugString( "*** function imageTtfBBox failed!" ); end
					
					end
					
					imageGif( ret, "tests/centered.gif", true );
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageCreate failed!" ); end
				
				end
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageTtfText ) then outputDebugString( "*** function imageTtfText doesn't exist!" ); end
				if( not imageTtfBBox ) then outputDebugString( "*** function imageTtfBBox doesn't exist!" ); end
			
			end
			
			return false;
		
		end
		
		function Test_imageDraw( theImg, theCol, theFill, theFile )
		
			if( imageLine and imageArc and imageRectangle and imageEllipse and imageFill ) then
			
				local ret = theImg or imageCreate( 128, 128 );
				if( ret ) then	
				
					imageColorAllocate( ret, 0, 0, 0 );
					local red = theFill or imageColorAllocate( ret, 255, 0, 0 );
					local green = theCol or imageColorAllocate( ret, 0, 255, 0 );
					
					local line = imageLine( ret, 0, 0, imageSX( ret ), imageSY( ret ), green );
					if( line ) then
					
						local arc = imageArc( ret, imageSX( ret )/2, imageSY( ret )/2, 100, 100, 225, 45, green );
						
						if( arc ) then
						
							local rectangle = imageRectangle( ret, 5, 5, imageSX( ret )-5, imageSY( ret )-5, green );
							if( rectangle ) then
							
								local ellipse = imageEllipse( ret, imageSX( ret )/2, imageSY( ret )/2, 35, 70, green );
								if( ellipse ) then
							
									local fill = imageFill( ret, imageSX( ret )/2 + 3, imageSY( ret )/2, red );
									if( fill ) then
									
										imagePng( ret, theFile or "tests/line_arc_rectangle_ellipse.png", 0, true );
										return true;
									
									else
									
										if( DEEP_OUTPUT ) then outputDebugString( "*** function imageFill failed!" ); end
									
									end
								
								else
								
									if( DEEP_OUTPUT ) then outputDebugString( "*** function imageEllipse failed!" ); end
								
								end								
							
							else
							
								if( DEEP_OUTPUT ) then outputDebugString( "*** function imageRectangle failed!" ); end
							
							end
						
						else
						
							if( DEEP_OUTPUT ) then outputDebugString( "*** function imageArc failed!" ); end
						
						end
					
					else
					
						if( DEEP_OUTPUT ) then outputDebugString( "*** function imageLine failed!" ); end
					
					end
					
					imageDestroy( ret );
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageCreate failed!" ); end
				
				end
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageLine ) then outputDebugString( "*** function imageLine doesn't exist!" ); end
				if( not imageArc ) then outputDebugString( "*** function imageArc doesn't exist!" ); end
				if( not imageRectangle ) then outputDebugString( "*** function imageRectangle doesn't exist!" ); end
				if( not imageEllipse ) then outputDebugString( "*** function imageEllipse doesn't exist!" ); end
				if( not imageFill ) then outputDebugString( "*** function imageFill doesn't exist!" ); end
			
			end
			
			return false;
		
		end
		
		function Test_imageDrawFilled( )
		
			if( imageFilledArc and imageFilledRectangle and imageFilledEllipse ) then
			
				local ret = imageCreate( 128, 128 );
				if( ret ) then	
				
					imageColorAllocate( ret, 0, 0, 0 );
					local red = imageColorAllocate( ret, 255, 0, 0 );
					local green = imageColorAllocate( ret, 0, 255, 0 );
					local blue = imageColorAllocate( ret, 0, 0, 255 );
					
					local arc = imageFilledArc( ret, imageSX( ret )/2, imageSY( ret )/2, 100, 100, 225, 45, red, 0 );
					
					if( arc ) then
					
						local rectangle = imageFilledRectangle( ret, 5, 5, imageSX( ret )/2, imageSY( ret )/2, green );
						if( rectangle ) then
						
							local ellipse = imageFilledEllipse( ret, imageSX( ret )/2, imageSY( ret )/2, 5, 20, blue );
							if( ellipse ) then
							
								imageGif( ret, "tests/Filled_arc_rectangle_ellipse.gif", true );
								return true;
							
							else
							
								if( DEEP_OUTPUT ) then outputDebugString( "*** function imageFilledEllipse failed!" ); end
							
							end								
						
						else
						
							if( DEEP_OUTPUT ) then outputDebugString( "*** function imageFilledRectangle failed!" ); end
						
						end
					
					else
					
						if( DEEP_OUTPUT ) then outputDebugString( "*** function imageFilledArc failed!" ); end
					
					end
					
					imageDestroy( ret );
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageCreate failed!" ); end
				
				end
			
			elseif( DEEP_OUTPUT ) then
			
				if( not imageFilledArc ) then outputDebugString( "*** function imageFilledArc doesn't exist!" ); end
				if( not imageFilledRectangle ) then outputDebugString( "*** function imageFilledRectangle doesn't exist!" ); end
				if( not imageFilledEllipse ) then outputDebugString( "*** function imageFilledEllipse doesn't exist!" ); end
			
			end
			
			return false;
		
		end
		
		function Test_imageStyle( )
		
			local ret = imageCreateTrueColor( 128, 128 );
			if( ret ) then
			
				imageColorAllocate( ret, 0, 0, 0 );
				local red = imageColorAllocate( ret, 255, 0, 0 );
				local green = imageColorAllocate( ret, 0, 255, 0 );
				local blue = imageColorAllocate( ret, 0, 0, 255 );
				local transparent = imageColorSpecial( "transparent" );
				
				if( imageSetStyle( ret, { red, green, blue, transparent, transparent, transparent } ) ) then
				
					return Test_imageDraw( ret, imageColorSpecial( "styled" ), nil, "tests/styled_lines.png" );
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageSetStyle failed!" ); end
				
				end
			
			end
			
			return false;
		
		end
		
		function Test_imageWide( )
		
			local ret = imageCreateTrueColor( 128, 128 );
			if( ret ) then
			
				imageColorAllocate( ret, 0, 0, 0 );
				
				if( imageSetThickness( ret, 10 ) ) then
				
					if( imageGetThickness( ret ) == 10 ) then
					
						return Test_imageDraw( ret, nil, nil, "tests/wide_lines.gif" );
					
					else
					
						if( DEEP_OUTPUT ) then outputDebugString( "*** function imageGetThickness failed!" ); end
					
					end
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageSetThickness failed!" ); end
				
				end
			
			end
			
			return false;
		
		end	
		
		function Test_imageTileBrush( )
		
			local ret = imageCreateTrueColor( 192, 192 );
			local tile = imageCreateFromJpeg( "tommylogo_normal.jpg" );
			local brush = imageCreateFromPng( "brush.png" );
			
			if( ret and tile and brush ) then
				
				imageSetSaveAlpha( ret, true );
				imageSetSaveAlpha( tile, true );
				imageSetSaveAlpha( brush, true );
				
				if( imageSetTile( ret, tile ) ) then
				
					imageFill( ret, imageSX( ret )/2 + 3, imageSY( ret )/2, imageColorSpecial( "tiled" ) );
					if( imageSetBrush( ret, brush ) ) then
					
						return Test_imageDraw( ret, imageColorSpecial( "brushed" ), imageColorSpecial( "tiled" ), "tests/tiled_brush.gif" );
					
					else
					
						if( DEEP_OUTPUT ) then outputDebugString( "*** function imageSetBrush failed!" ); end
					
					end
				
				else
				
					if( DEEP_OUTPUT ) then outputDebugString( "*** function imageSetTile failed!" ); end
				
				end
				
				imageDestroy( ret );
				imageDestroy( tile );
				imageDestroy( brush );
			
			end
			
			return false;
		
		end	
	
		local theTests = {
		
			Test_gd_Info,
			Test_imageCreate,
			Test_imageCreateTrueColor,
			Test_imageCreateFromPng,
			Test_imageCreateFromGif,
			Test_imageCreateFromJpeg,
			Test_imageSXY,
			Test_imageIsTrueColor,
			Test_imageSave,
			Test_imageAlphaBlending,
			Test_imageAntiAlias,
			Test_imageInterlace,
			Test_imageSaveAlpha,
			Test_imagePaletteColor,
			Test_imageColorSpecial,
			Test_imageFont,
			Test_imageChar,
			Test_imageString,
			Test_imageTTF,
			Test_imageDraw,
			Test_imageDrawFilled,
			Test_imageStyle,
			Test_imageWide,
			Test_imageTileBrush,
		
		};
		
		function doTests( )
		
			outputDebugString( "Starting Tests" );
			outputDebugString( "--------------------------------------" );
			
			local success = 0;
			local total = 0;
			
			for k, v in ipairs( theTests ) do
			
				total = total + 1;
				if( DEEP_OUTPUT ) then outputDebugString( string.format( "\tTest #%d: Starting", total, resString ) ); end
				
				local result = v();
				local resString = "FAIL";
				if( result ) then success = success + 1; resString = "SUCCESS"; end
				
				outputDebugString( string.format( "\tTest #%d: %s", total, resString ) );
			
			end
			
			outputDebugString( "--------------------------------------" );
			outputDebugString( "End of Tests" );
			outputDebugString( "Summary:" );
			outputDebugString( success .. " out of " .. total .. " tests were successful." );
			outputDebugString( "Check images in tests folder..." );
			
		
		end
		
		addEventHandler( "onResourceStart", getResourceRootElement( getThisResource( ) ), doTests );

	end

end