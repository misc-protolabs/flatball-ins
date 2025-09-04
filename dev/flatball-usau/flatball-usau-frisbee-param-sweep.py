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
import FreeCADGui as Gui  # Import the FreeCAD GUI module
from PySide import QtGui # Import QtGui from PySide
import Part
import PartDesign
import Sketcher
import Draft
import math
import csv
#from openpyxl import Workbook
#pip install openpyxl

# Frisbee parameters
m = 175             # --> 175g +/- 3g [172 .. 178]g
d = 274             # --> 274mm +/-3mm [271 .. 277]mm
h = 32              # --> 32mm +/- 2mm [30 .. 34]mm
rho = 0.925*1000    # --> kg/m^3 LLDPE --> [0.915 .. 0.925] g/cm^3

def mk_frisbee(doc):
    body = doc.addObject('PartDesign::Body', 'frisbee')
    sketch = frisbee_x_section_sketch(body)
    revolution = revolve_frisbee_x_section_sketch(body, sketch)

    return body, sketch, revolution

def frisbee_x_section_sketch(body):
    sketch = body.newObject('Sketcher::SketchObject', 'flatball-usau-cross-section')
    sketch.AttachmentOffset = FreeCAD.Placement(FreeCAD.Vector(0, 0, 0), FreeCAD.Rotation(0, 0, 0))

    disc_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h1, 0), FreeCAD.Vector(x1, h1, 0)))
    disc_l2 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h2, 0), FreeCAD.Vector(x2, h2, 0)))
    vert_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h1, 0), FreeCAD.Vector(0, h2, 0)))

    ellipse1 = Part.Ellipse(FreeCAD.Vector(x1, y1, 0), a, b)
    ellipse2 = Part.Ellipse(FreeCAD.Vector(x2, y2, 0), a, b)
    ext_foil_arc = Part.ArcOfEllipse(ellipse1, ext_foil_th1, ext_foil_th2)
    int_foil_arc = Part.ArcOfEllipse(ellipse2, int_foil_th1, int_foil_th2)
    ext_foil = sketch.addGeometry(ext_foil_arc)
    int_foil = sketch.addGeometry(int_foil_arc)
    rimh_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(x3 - w_rim, 0, 0), FreeCAD.Vector(x3, 0, 0)))
    rimv_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(x3 - w_rim, y3, 0), FreeCAD.Vector(x4, y4, 0)))

    doc.recompute()
    return sketch

def revolve_frisbee_x_section_sketch(body, sketch):
    revolution = body.newObject('PartDesign::Revolution', 'revolution')
    revolution.Profile = sketch
    revolution.ReferenceAxis = (sketch, 'V_Axis')  # Use vertical axis by default
    revolution.Angle = 360.0

    doc.recompute()
    return revolution

def frisbee_metrics(shape, file_name="flatball-usau-frisbee-param-sweep.csv"):
    surface_area = shape.Area
    volume = shape.Volume
    center_of_mass = shape.CenterOfMass

    volume_m3 = volume * 1e-9  # mm^3 to m^3
    mass = rho * volume_m3 * 1e3  # g

    inertia_tensor = shape.MatrixOfInertia
    inertia_tensor.scale(mass / shape.Volume)

    if 172 <= mass <= 178:
        with open(file_name, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([
                f"{d1:.3f}", f"{h1:.3f}", f"{h2:.3f}", f"{major_diameter:.3f}", f"{minor_diameter:.3f}", f"{w_rim:.3f}",
                f"{mass:.3f}", f"{inertia_tensor.A22:.3f}", f"{inertia_tensor.A11:.3f}",
                f"{center_of_mass.y:.3f}",
                f"{volume:.3f}", f"{inertia_tensor.A33:.3f}",
                f"{surface_area:.3f}", f"{center_of_mass.x:.3f}", f"{center_of_mass.z:.3f}"
            ])

# Create a CSV file with headers
with open("flatball-usau-frisbee-param-sweep.csv", mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["d", "h1", "h2", "d-maj", "d-min", "w-rim",
        "Mass (g)", "Iyy (g·mm²)", "Ixx (g·mm²)",
        "Center of Mass y (mm)",
        "Volume (mm^3)", "Izz (g·mm²)",
        "Surface Area (mm^2)", "Center of Mass x (mm)", "Center of Mass z (mm)"
        ])


doc = FreeCAD.newDocument(f"flatball-usau-frisbee-param-sweep")
for d1_ in [round(idx1, 1) for idx1 in range(2710, 2771)]:
    for h1_ in [round(idx2, 1) for idx2 in range(300, 341)]:
        for d_maj_ in [round(idx3, 1) for idx3 in range(698, 703)]:
            for d_min_ in [round(idx4, 1) for idx4 in range(458, 463)]:
                for w_rim_ in [round(idx5, 1) for idx5 in range(31, 36)]:
                    d1 = d1_ * 1e-1
                    d2 = 270  # mm

                    h1 = h1_* 1e-1 # mm
                    h_plate = 1.85 # mm
                    h2 = h1 - h_plate # mm

                    # Foil parameters
                    major_diameter = d_maj_ * 1e-1  # mm
                    minor_diameter = d_min_ * 1e-1  # mm
                    w_rim = w_rim_ * 1e-1 # mm - rim width

                    a = major_diameter / 2  # mm - semi-major axis
                    b = minor_diameter / 2  # mm - semi-minor axis

                    ext_foil_th1 = math.asin(-(h1 - b) / b)
                    ext_foil_th2 = math.pi / 2
                    ext_foil_th3 = 0
                    int_foil_th1 = math.pi / 8
                    int_foil_th2 = math.pi / 2

                    x1 = (d1 - major_diameter) / 2
                    y1 = (h1 - b)
                    x2 = (d2 - major_diameter) / 2
                    y2 = (h2 - b)
                    x3 = x1 + a * math.cos(ext_foil_th1)
                    y3 = y1 + b * math.sin(ext_foil_th1)
                    x4 = x2 + a * math.cos(int_foil_th1)
                    y4 = y2 + b * math.sin(int_foil_th1)
                    x5 = x1 + a * math.cos(ext_foil_th3)
                    y5 = y1 + b * math.sin(ext_foil_th3)

                    print( f"d : {d1:5.3f} h : {h1:5.3f}, h2 {h2:5.3f}, d-maj {major_diameter:5.3f}, d-min {minor_diameter:5.3f}, w-rim {w_rim:5.3f}")

                    frisbee, sketch, revolution = mk_frisbee(doc)
                    frisbee_metrics(revolution.Shape)

                    doc.removeObject(frisbee.Name)
                    doc.removeObject(sketch.Name)
                    doc.removeObject(revolution.Name)
