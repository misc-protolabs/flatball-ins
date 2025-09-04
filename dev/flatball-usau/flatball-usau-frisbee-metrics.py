# Copyright 2025 Michael V. Schaefer
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import FreeCAD
import Import
import Part

# Open your STEP file in the active document
Import.open('USAU-frisbee.STEP')

# Get the active document
doc = FreeCAD.ActiveDocument

# Check if any objects were imported
if doc.Objects:
    shape = doc.Objects[0].Shape

    # Calculate volume
    volume = shape.Volume  # in cubic millimeters

    # Define density (in grams per cubic millimeter)
    #density = 0.008  # for example, steel has a density of ~0.008 g/mm^3
    density_LLDPE = 0.925 * 1e-3  # g/mm^3

    # Calculate mass
    mass = volume * density_LLDPE  # in grams

    print(f"Volume: {volume} mm³")
    print(f"Density: {density_LLDPE} g/mm³")
    print(f"Mass: {mass} g")
else:
    print("No objects found in the document. Please check the file path and ensure the STEP file is valid.")
