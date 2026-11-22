# Corstone KWS regression

Use an NSX checkout with the Corstone NPU module registered, Arm GNU, the board's licensed FVP and Vela 5.2.0. This test runs one complete keyword-spotting command stream with 490 zero input bytes. It compares all 12 output bytes with the original model's LiteRT reference-kernel output and requires a successful dispatch with positive NPU active cycles.

Run from this provider checkout. For Corstone-300 substitute `corstone300_fvp_u55` and `ethos-u55-32`. Use a fresh output directory per target.

```sh
board=corstone320_fvp_u85
arch=ethos-u85-256
app="$PWD/kws"
nsx create-app "$app" --board "$board"
nsx module add nsx-corstone-npu --app-dir "$app"
curl -fL https://ambiqai-model-zoo.s3-us-west-2.amazonaws.com/helia-estimator/models/kws_ref_model.tflite -o "$app/kws.tflite"
(cd "$app" && echo 'aeea436800704fce17b17292e4412630ad856e9d777c044c64ef748a880bd0ae  kws.tflite' | sha256sum -c -)
vela --accelerator-config "$arch" --memory-mode Shared_Sram --output-format raw --output-dir "$app/compiled" "$app/kws.tflite"
python tests/kws/prepare.py "$app/compiled/kws_vela.npz" "$app/src/payload.h"
cp tests/kws/main.c "$app/src/main.c"
echo 'target_link_libraries(kws PRIVATE nsx::corstone_npu)' >> "$app/CMakeLists.txt"
nsx build "$app"
nsx run "$app" --fvp /path/to/model
```

To check failure propagation, append `target_compile_definitions(kws PRIVATE KWS_NEGATIVE_CONTROL)` to the app CMake file, rebuild and rerun. The command must exit 1 after the dispatch. Counts depend on the FVP configuration and serve as evidence of execution. The fixture uses the simulator's default memory timing.
