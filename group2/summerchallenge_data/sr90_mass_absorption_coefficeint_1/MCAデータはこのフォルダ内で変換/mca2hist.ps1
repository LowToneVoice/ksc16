###########################
## mca to hist converter ##
##  2020/09/01 s.nagao   ##
###########################
$mca_files = Get-ChildItem -Recurse -File -Include *.mca    # search mca files
#Write-Host($mca_files.Length)

foreach($mca in $mca_files){    # roop in mca files
  $ifname = $mca
  $ofname = ($mca.Basename + ".hist")

  if( $(test-path $ifname) -ne $True ){
#      Write-Error("ERROR: Not found " + $ifname)
      Write-Host("ERROR: Not found " + $ifname) -BackgroundColor Red
      continue
  }
  if( $(test-path $ofname) -eq $True ){
#      Write-Error("ERROR: Can't overwrite " + $ofname)
      Write-Host("ERROR: Can't overwrite " + $ofname) -BackgroundColor Red
      continue
  }

  Write-Host("converting " + $mca.Basename + ".mca to " + $mca.Basename + ".hist")

  Set-Content -Path "$ofname" -Value $str   # Make a new hist file
  $counts = 0
  $bin = 1

  $lines = get-content $ifname
  foreach($line in $lines){
    if ($line -match "THRESHOLD"){
      $val = "# " + $line
      Add-Content -Path "$ofname" -Value $val
    }
    elseif ($line -match "REAL_TIME"){
      $val = "# " + $line
      Add-Content -Path "$ofname" -Value $val
    }
    elseif ($line -match "START_TIME"){
      $val = "# " + $line
      Add-Content -Path "$ofname" -Value $val
    }
    elseif ([int]::TryParse($line,[ref]$counts)){
      $val = [string]$bin + " " + [string]$counts
      Add-Content -Path "$ofname" -Value $val
      $bin++
    }
  }  # for $lines
}  # for $mca_files

