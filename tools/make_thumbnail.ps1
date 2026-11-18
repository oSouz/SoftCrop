Add-Type -AssemblyName System.Drawing
$assetDir = Join-Path $PSScriptRoot '../assets'
$logo = [System.Drawing.Image]::FromFile((Join-Path $assetDir 'souz-branco.png'))
$canvas = [System.Drawing.Bitmap]::new(640,360)
$g = [System.Drawing.Graphics]::FromImage($canvas)
$g.Clear([System.Drawing.Color]::FromArgb(255,12,12,15))
$g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
$g.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::AntiAliasGridFit
$logoHeight = [int](400 * $logo.Height / $logo.Width)
$g.DrawImage($logo,120,85,400,$logoHeight)
$font = [System.Drawing.Font]::new('Segoe UI Light',46,[System.Drawing.FontStyle]::Regular,[System.Drawing.GraphicsUnit]::Pixel)
$format = [System.Drawing.StringFormat]::new()
$format.Alignment = [System.Drawing.StringAlignment]::Center
$g.DrawString('SoftCrop',$font,[System.Drawing.Brushes]::White,[System.Drawing.RectangleF]::new(0,235,640,80),$format)
$canvas.Save((Join-Path $assetDir 'SoftCrop-preview.png'),[System.Drawing.Imaging.ImageFormat]::Png)
$thumb = [System.Drawing.Bitmap]::new(640,360)
$tg = [System.Drawing.Graphics]::FromImage($thumb)
$tg.Clear([System.Drawing.Color]::FromArgb(255,12,12,15))
$tg.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$tg.DrawImage($canvas,0,0,640,360)
$thumb.Save((Join-Path $assetDir 'SoftCrop-thumbnail.png'),[System.Drawing.Imaging.ImageFormat]::Png)
$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add('// Generated from assets/SoftCrop-thumbnail.png by tools/make_thumbnail.ps1')
$lines.Add('// SDK CFFGLColor 0xRRGGBBAA layout, matching the user GridPanel implementation.')
$lines.Add('static const CFFGLColor souzThumbnailPixels[] = {')
for($y=0;$y -lt 360;$y++) {
    $row = [System.Collections.Generic.List[string]]::new()
    for($x=0;$x -lt 640;$x++) {
        $c = $thumb.GetPixel($x,$y)
        $row.Add(('CFFGLColor(0x{0:X2}{1:X2}{2:X2}{3:X2}u)' -f $c.R,$c.G,$c.B,$c.A))
    }
    $lines.Add(($row -join ',')+',')
}
$lines.Add('};')
[System.IO.File]::WriteAllLines((Join-Path $PSScriptRoot '../src/SouzThumbnail.h'),$lines)
$tg.Dispose(); $thumb.Dispose(); $format.Dispose(); $font.Dispose(); $g.Dispose(); $canvas.Dispose(); $logo.Dispose()
