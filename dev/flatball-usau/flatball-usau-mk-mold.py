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
import json
import os

# Frisbee parameters
m = 175 # --> 175g +/- 3g [172 .. 178]g
d1 = 276  # --> 274mm +/-3mm [271 .. 277]mm
h1 = 31   # --> 32mm +/- 2mm [30 .. 34]mm

h_platter = 1.75 # mm
d2 = 272  # mm
h2 = h1 - h_platter  # mm

rho_lldpe = 0.925 * 1000  # kg/m^3 --> [0.915 .. 0.925] g/cm^3
rho_titanium_dioxide = 4.26 * 1000 # kg/m^3 --> 4.26 g/cm^3
pct_titatium_dioxide = 0.015; # --> [1..2]%
rho = rho_lldpe * (1-pct_titatium_dioxide) + rho_titanium_dioxide * pct_titatium_dioxide

# Foil parameters
major_diameter = 70  # mm
minor_diameter = 46   # mm
w_rim = 3.75  # mm - rim width

a = major_diameter / 2  # mm - semi-major axis
b = minor_diameter / 2  # mm - semi-minor axis

ext_foil_th1 = math.asin(-(h1 - b) / b)
ext_foil_th2 = math.pi / 2
ext_foil_th3 = 0
int_foil_th1 = math.pi / 9
int_foil_th2 = math.pi / 2

# Sketch coordinates
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

# Mold parameters
mold_x_margin = 10
mold_y_margin = 10
w_sprew_tip = (h1-h2) * 1.5
w_sprew_gate = w_sprew_tip + (h2+mold_y_margin)*math.sin(3/180*math.pi)

def mk_frisbee():
    body = doc.addObject('PartDesign::Body', 'frisbee')
    sketch = frisbee_x_section_sketch( body)
    revolution = revolve_frisbee_x_section_sketch(body, sketch)
    #revolution.Material = 'flatball-usau-lldpe-mix'
    frisbee_metrics(revolution.Shape)
    Part.export([body], "flatball-usau-frisbee.stl")
    Part.export([body], "flatball-usau-frisbee.step")
    #print(f"Model(s) exported to stl files.")

    return body

def mk_mold_cavity():
    body = doc.addObject('PartDesign::Body', 'mold_cavity')
    sketch = mold_cavity_x_section_sketch( body)
    revolution = revolve_mold_cavity_x_section_sketch(body, sketch)
    translation = App.Vector(0, 50, 0)
    body.Placement.Base = body.Placement.Base + translation    
    Part.export([body], "flatball-usau-mold-cavity.stl")
    Part.export([body], "flatball-usau-mold-cavity.step")
    #print(f"Model(s) exported to stl files.")
    return body
        
def mk_mold_core():
    body = doc.addObject('PartDesign::Body', 'mold_core')
    sketch = mold_core_x_section_sketch( body)
    revolution = revolve_mold_core_x_section_sketch(body, sketch)
    translation = App.Vector(0, -50, 0)
    body.Placement.Base = body.Placement.Base + translation    
    Part.export([body], "flatball-usau-mold-core.stl")
    Part.export([body], "flatball-usau-mold-core.step")
    #print(f"Model(s) exported to stl files.")
    return body

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

    #ellipse1 = Part.Ellipse(FreeCAD.Vector(-x1, y1, 0), a, b)
    #ellipse2 = Part.Ellipse(FreeCAD.Vector(-x2, y2, 0), a, b)
    #ext_foil_arc = Part.ArcOfEllipse(ellipse1, ext_foil_th2, math.pi-ext_foil_th1)
    #int_foil_arc = Part.ArcOfEllipse(ellipse2, int_foil_th2, math.pi-int_foil_th1)
    #ext_foil = sketch.addGeometry(ext_foil_arc)
    #int_foil = sketch.addGeometry(int_foil_arc)
    #rimh_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(-x3 + w_rim, 0, 0), FreeCAD.Vector(-x3, 0, 0)))
    #rimv_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(-x3 + w_rim, y3, 0), FreeCAD.Vector(-x4, y4, 0)))

    #constrain_frisbee_x_section_sketch(sketch, disc_l1, disc_l2, vert_l1, ext_foil, int_foil, rimh_l1, rimv_l1, h1, h2, w_rim)

    doc.recompute()
    return sketch

def constrain_frisbee_x_section_sketch(sketch, disc_l1, disc_l2, vert_l1, ext_foil, int_foil, rimh_l1, rimv_l1, h1, h2, w_rim):
    sketch.addConstraint(Sketcher.Constraint('Vertical', vert_l1))
    sketch.addConstraint(Sketcher.Constraint('Horizontal', rimh_l1))
    sketch.addConstraint(Sketcher.Constraint('Tangent', disc_l1, 2, ext_foil, 2))
    sketch.addConstraint(Sketcher.Constraint('Tangent', disc_l2, 2, int_foil, 2))
    sketch.addConstraint(Sketcher.Constraint('Coincident', disc_l1, 1, vert_l1, 1))
    sketch.addConstraint(Sketcher.Constraint('Coincident', disc_l2, 1, vert_l1, 2))
    sketch.addConstraint(Sketcher.Constraint('Coincident', rimh_l1, 2, ext_foil, 1))
    sketch.addConstraint(Sketcher.Constraint('Coincident', rimh_l1, 1, rimv_l1, 1))
    sketch.addConstraint(Sketcher.Constraint('Coincident', rimv_l1, 2, int_foil, 1))
    sketch.addConstraint(Sketcher.Constraint('DistanceY', rimh_l1, 2, disc_l1, 2, h1))
    sketch.addConstraint(Sketcher.Constraint('DistanceY', disc_l2, 1, disc_l1, 1, (h1 - h2)))
    sketch.addConstraint(Sketcher.Constraint('DistanceX', rimh_l1, 1, rimh_l1, 2, w_rim))

    doc.recompute()

def revolve_frisbee_x_section_sketch(body, sketch):
    revolution = body.newObject('PartDesign::Revolution', 'revolution')
    revolution.Profile = sketch
    revolution.ReferenceAxis = (sketch, 'V_Axis')  # Use vertical axis by default
    revolution.Angle = 360.0

    doc.recompute()
    return revolution

def create_custom_material():
    # Define the LLDPE material properties
    material_props = {
        'Name': 'LLDPE',
        'Description': 'Linear Low-Density Polyethylene',
        'Density': 0.925 * 1000,  # kg/m^3
        'YoungsModulus': 200,  # MPa (megapascal)
        'PoissonsRatio': 0.42,  # Dimensionless
        'YieldStrength': 10,  # MPa (megapascal)
        'UltimateTensileStrength': 17,  # MPa (megapascal)
        'ThermalConductivity': 0.45,  # W/m·K
        'SpecificHeat': 2200,  # J/kg·K
        'ThermalExpansionCoefficient': 180e-6,  # 1/K
        'Color': (0.8, 0.8, 0.8),  # RGB values between 0 and 1
    }

    # Create a new material in the active document
    material = FreeCAD.ActiveDocument.addObject("App::MaterialObjectPython", "LLDPE_Material")

    # Assign the properties to the material object
    material.Label = material_props['Name']
    material.addProperty("App::PropertyString", "Description", "Description")
    material.Description = material_props['Description']
    material.addProperty("App::PropertyFloat", "Density", "Density")
    material.Density = material_props['Density']
    material.addProperty("App::PropertyFloat", "YoungsModulus", "Young's Modulus")
    material.YoungsModulus = material_props['YoungsModulus']
    material.addProperty("App::PropertyFloat", "PoissonsRatio", "Poisson's Ratio")
    material.PoissonsRatio = material_props['PoissonsRatio']
    material.addProperty("App::PropertyFloat", "YieldStrength", "Yield Strength")
    material.YieldStrength = material_props['YieldStrength']
    material.addProperty("App::PropertyFloat", "UltimateTensileStrength", "Ultimate Tensile Strength")
    material.UltimateTensileStrength = material_props['UltimateTensileStrength']
    material.addProperty("App::PropertyFloat", "ThermalConductivity", "Thermal Conductivity")
    material.ThermalConductivity = material_props['ThermalConductivity']
    material.addProperty("App::PropertyFloat", "SpecificHeat", "Specific Heat")
    material.SpecificHeat = material_props['SpecificHeat']
    material.addProperty("App::PropertyFloat", "ThermalExpansionCoefficient", "Thermal Expansion Coefficient")
    material.ThermalExpansionCoefficient = material_props['ThermalExpansionCoefficient']
    material.addProperty("App::PropertyColor", "Color", "Color")
    material.Color = material_props['Color']

    # Recompute the document to apply changes
    FreeCAD.ActiveDocument.recompute()

    # Save the material properties to a .FCMat file
    save_material_to_file(material_props)

    return material

def save_material_to_file(material_props):
    # Define the file path
    file_path = os.path.join(os.path.expanduser("~"), "LLDPE_Material.FCMat")

    # Create a string to store material properties in the format required by .FCMat files
    material_str = f"""Material:
  Name: {material_props['Name']}
  Description: {material_props['Description']}
  Density: {{Value: {material_props['Density']}, Unit: 'kg/m^3'}}
  YoungsModulus: {{Value: {material_props['YoungsModulus']}, Unit: 'MPa'}}
  PoissonsRatio: {{Value: {material_props['PoissonsRatio']}, Unit: ''}}
  YieldStrength: {{Value: {material_props['YieldStrength']}, Unit: 'MPa'}}
  UltimateTensileStrength: {{Value: {material_props['UltimateTensileStrength']}, Unit: 'MPa'}}
  ThermalConductivity: {{Value: {material_props['ThermalConductivity']}, Unit: 'W/m·K'}}
  SpecificHeat: {{Value: {material_props['SpecificHeat']}, Unit: 'J/kg·K'}}
  ThermalExpansionCoefficient: {{Value: {material_props['ThermalExpansionCoefficient']}, Unit: '1/K'}}
  Color: [{material_props['Color'][0]}, {material_props['Color'][1]}, {material_props['Color'][2]}]
"""

    # Write the material properties to the .FCMat file
    with open(file_path, 'w') as file:
        file.write(material_str)

    print(f"Custom material saved to {file_path}")

def frisbee_metrics(shape, file_name="flatball-usau-frisbee-metrics.txt"):
    surface_area = shape.Area # mm^2
    volume = shape.Volume # mm^3
    center_of_mass = shape.CenterOfMass # mm

    volume_m3 = volume * 1e-9  # mm^3 to m^3
    mass = rho * volume_m3 # kg

    inertia_tensor = shape.MatrixOfInertia # kg*mm^2
    inertia_tensor.scale(1e-6) # kg*m^2
    inertia_tensor.scale(1e-6) # ?WTF?
    #inertia_tensor.scale(volume_m3 / mass) #(mass / shape.Volume)
    
    with open(file_name, "w") as file:
        file.write(f"Surface Area: {surface_area:.2f} mm^2\n")
        file.write(f"Volume: {volume:.2f} mm^3\n")
        file.write(f"Mass: {mass:.4f} kg\n")
        file.write("Center of Mass: x = {:.2f} mm, y = {:.2f} mm, z = {:.2f} mm\n".format(center_of_mass.x, center_of_mass.y, center_of_mass.z))
        file.write("Moment of Inertia: Ixx = {:.3e} g·mm², Iyy = {:.3e} g·mm², Izz = {:.3e} g·mm²\n".format(inertia_tensor.A11, inertia_tensor.A22, inertia_tensor.A33))

    #print(f"Surface Area: {surface_area:.2f} mm^2")
    #print(f"Volume: {volume:.2f} mm^3")
    #print(f"Mass: {mass:.4f} kg")
    #print(f"Mass: {mass:.4f} kg, Volume: {volume:.2f} mm^3, Surface Area: {surface_area:.2f} mm^2")
    #print("Center of Mass: x = {:.2f} mm, y = {:.2f} mm, z = {:.2f} mm".format(center_of_mass.x, center_of_mass.y, center_of_mass.z))
    #print("Moment of Inertia: Ixx = {:.2f} g·mm², Iyy = {:.2f} g·mm², Izz = {:.2f} g·mm²".format(inertia_tensor.A11, inertia_tensor.A22, inertia_tensor.A33))
    print( f"m = {mass:7.4f} kg, h = {h1:5.3f} mm, CofM = {center_of_mass.y:5.3f} mm, Volume = {volume_m3:5.3e} m^3")
    print( f"Ixx = {inertia_tensor.A11:5.3e} kg·m², Iyy = {inertia_tensor.A22:5.3e} kg·m², Izz = {inertia_tensor.A33:5.3e} kg·m²")

def mold_cavity_x_section_sketch(body):
    sketch = body.newObject('Sketcher::SketchObject', 'flatball-usau-mold-top-cross-section')
    sketch.AttachmentOffset = FreeCAD.Placement(FreeCAD.Vector(0, 0, 0), FreeCAD.Rotation(0, 0, 0))

    disc_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h1, 0), FreeCAD.Vector(x1, h1, 0)))
    disc_l2 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h1+mold_y_margin, 0), FreeCAD.Vector(d1/2 + mold_x_margin, h1+mold_y_margin, 0)))
    vert_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(0, h1+mold_y_margin, 0), FreeCAD.Vector(0, h1, 0)))
    vert_l2 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(d1/2 + mold_x_margin, h1+mold_y_margin, 0), FreeCAD.Vector(d1/2 + mold_y_margin, y1, 0)))
    ellipse1 = Part.Ellipse(FreeCAD.Vector(x1, y1, 0), a, b)
    ext_foil_arc = Part.ArcOfEllipse(ellipse1, ext_foil_th3, ext_foil_th2)
    ext_foil = sketch.addGeometry(ext_foil_arc)
    part_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(d1/2 + mold_x_margin, y5, 0), FreeCAD.Vector(x5, y5, 0)))

    doc.recompute()
    return sketch

def revolve_mold_cavity_x_section_sketch(body, sketch):
    revolution = body.newObject('PartDesign::Revolution', 'revolution')
    revolution.Profile = sketch
    revolution.ReferenceAxis = (sketch, 'V_Axis')  # Use vertical axis by default
    revolution.Angle = 360.0

    doc.recompute()
    return revolution

def mold_core_x_section_sketch(body):
    sketch = body.newObject('Sketcher::SketchObject', 'flatball-usau-mold-bot-cross-section')
    sketch.AttachmentOffset = FreeCAD.Placement(FreeCAD.Vector(0, 0, 0), FreeCAD.Rotation(0, 0, 0))

    disc_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(w_sprew_gate, -mold_y_margin, 0), FreeCAD.Vector(d1/2 + mold_x_margin, -mold_y_margin, 0)))
    disc_l2 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(w_sprew_tip, h2, 0), FreeCAD.Vector(x2, h2, 0)))
    vert_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(w_sprew_tip, h2, 0), FreeCAD.Vector(w_sprew_gate, -mold_y_margin, 0)))
    ellipse1 = Part.Ellipse(FreeCAD.Vector(x1, y1, 0), a, b)
    ellipse2 = Part.Ellipse(FreeCAD.Vector(x2, y2, 0), a, b)
    ext_foil_arc = Part.ArcOfEllipse(ellipse1, ext_foil_th1, ext_foil_th3)
    ext_foil = sketch.addGeometry(ext_foil_arc)
    int_foil_arc = Part.ArcOfEllipse(ellipse2, int_foil_th1, int_foil_th2)
    int_foil = sketch.addGeometry(int_foil_arc)
    rimh_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(x3 - w_rim, 0, 0), FreeCAD.Vector(x3, 0, 0)))
    rimv_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(x3 - w_rim, y3, 0), FreeCAD.Vector(x4, y4, 0)))
    part_l1 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(d1/2 + mold_x_margin, y5, 0), FreeCAD.Vector(x5, y5, 0)))
    vert_l2 = sketch.addGeometry(Part.LineSegment(FreeCAD.Vector(d1/2 + mold_x_margin, y5, 0), FreeCAD.Vector(d1/2 + mold_x_margin, -mold_y_margin, 0)))

    doc.recompute()
    return sketch

def revolve_mold_core_x_section_sketch(body, sketch):
    revolution = body.newObject('PartDesign::Revolution', 'revolution')
    revolution.Profile = sketch
    revolution.ReferenceAxis = (sketch, 'V_Axis')  # Use vertical axis by default
    revolution.Angle = 360.0

    doc.recompute()
    return revolution

def mk_mold_cavity_base( ):
    body = doc.addObject('PartDesign::Body', 'mold_cavity_base')
    sketch = body.newObject('Sketcher::SketchObject', 'Sketch')

    cylinder_diameter = d1+mold_x_margin*2
    cylinder_depth = (h1+mold_y_margin - y1)
    length = d1+mold_x_margin*4
    width = d1+mold_x_margin*4
    height = cylinder_depth + mold_y_margin

    # Set the placement of the sketch to align with the XZ plane
    sketch.Placement = App.Placement(App.Vector(0, 0, 0), App.Rotation(App.Vector(1, 0, 0), 90))

    # Define the corners of the box, centered on the origin
    points = [
        FreeCAD.Vector(-length / 2, -width / 2, 0),
        FreeCAD.Vector(length / 2, -width / 2, 0),
        FreeCAD.Vector(length / 2, width / 2, 0),
        FreeCAD.Vector(-length / 2, width / 2, 0),
    ]

    # Add lines between the corners to create the box
    for i in range(len(points) - 1):
        sketch.addGeometry(Part.LineSegment(points[i], points[i + 1]), False)

    # Add the final line to close the box
    sketch.addGeometry(Part.LineSegment(points[-1], points[0]))

    # Recompute the document to apply the changes
    doc.recompute()

    # Extrude the sketch to create a 3D solid
    extrude = body.newObject('PartDesign::Pad', 'Pad')
    extrude.Profile = sketch
    extrude.Length = height  # Set the extrusion length (height of the box)

    # Recompute the document to apply the changes
    doc.recompute()

    # Create a cylinder
    cylinder = Part.makeCylinder(cylinder_diameter / 2, cylinder_depth)

    # Rotate the cylinder to align with the XZ plane
    cylinder.rotate(FreeCAD.Vector(0, 0, 0), FreeCAD.Vector(1, 0, 0), 90)

    # Move the cylinder to the desired position (flush with the top surface of the box)
    box_center = body.Shape.BoundBox.Center
    cylinder.translate(FreeCAD.Vector(box_center.x, 0, box_center.z))

    # Cut the cylinder from the box
    box_shape = body.Shape.cut(cylinder)

    # Convert the resulting shape to a PartDesign feature and update the body
    base_feature = body.newObject("PartDesign::Feature", "CutCylinder")
    base_feature.Shape = box_shape

    # Apply translation
    translation = App.Vector(0, 50 + height - cylinder_depth, 0)
    body.Placement.Base = body.Placement.Base + translation    
    
    # Apply rotation
    rotation = App.Rotation(App.Vector(0, 0, 1), 180)
    body.Placement.Rotation = rotation.multiply(body.Placement.Rotation)
    
    # Recompute the document to apply the changes
    doc.recompute()

    Part.export([body], "flatball-usau-mold-cavity-base.stl")
    Part.export([body], "flatball-usau-mold-cavity-base.step")
    #print(f"Model(s) exported to stl files.")
    
    return body
    

def mk_mold_core_base( ):
    body = doc.addObject('PartDesign::Body', 'mold_core_base')
    sketch = body.newObject('Sketcher::SketchObject', 'Sketch')

    cylinder_diameter = d1+mold_x_margin*2
    cylinder_depth = (y5 + mold_y_margin)
    length = d1+mold_x_margin*4
    width = d1+mold_x_margin*4
    height = cylinder_depth + mold_y_margin

    # Set the placement of the sketch to align with the XZ plane
    sketch.Placement = App.Placement(App.Vector(0, 0, 0), App.Rotation(App.Vector(1, 0, 0), 90))

    # Define the corners of the box, centered on the origin
    points = [
        FreeCAD.Vector(-length / 2, -width / 2, 0),
        FreeCAD.Vector(length / 2, -width / 2, 0),
        FreeCAD.Vector(length / 2, width / 2, 0),
        FreeCAD.Vector(-length / 2, width / 2, 0),
    ]

    # Add lines between the corners to create the box
    for i in range(len(points) - 1):
        sketch.addGeometry(Part.LineSegment(points[i], points[i + 1]), False)

    # Add the final line to close the box
    sketch.addGeometry(Part.LineSegment(points[-1], points[0]))

    # Recompute the document to apply the changes
    doc.recompute()

    # Extrude the sketch to create a 3D solid
    extrude = body.newObject('PartDesign::Pad', 'Pad')
    extrude.Profile = sketch
    extrude.Length = height  # Set the extrusion length (height of the box)

    # Recompute the document to apply the changes
    doc.recompute()

    # Create a cylinder
    cylinder = Part.makeCylinder(cylinder_diameter / 2, cylinder_depth)

    # Rotate the cylinder to align with the XZ plane
    cylinder.rotate(FreeCAD.Vector(0, 0, 0), FreeCAD.Vector(1, 0, 0), 90)

    # Move the cylinder to the desired position (flush with the top surface of the box)
    box_center = body.Shape.BoundBox.Center
    cylinder.translate(FreeCAD.Vector(box_center.x, 0, box_center.z))# + height / 2 - cylinder_depth))

    # Cut the cylinder from the box
    box_shape = body.Shape.cut(cylinder)

    # Convert the resulting shape to a PartDesign feature and update the body
    base_feature = body.newObject("PartDesign::Feature", "CutCylinder")
    base_feature.Shape = box_shape

    # Apply translation
    translation = App.Vector(0, -50 + height - cylinder_depth, 0)
    body.Placement.Base = body.Placement.Base + translation    
        
    # Recompute the document to apply the changes
    doc.recompute()

    Part.export([body], "flatball-usau-mold-core-base.stl")
    Part.export([body], "flatball-usau-mold-core-base.step")
    #print(f"Model(s) exported to stl files.")
    
    return body
    
# Create the main document
doc = FreeCAD.newDocument("flatball-usau")

#custom_material = create_custom_material()
frisbee = mk_frisbee()
mold_cavity_base = mk_mold_cavity_base()
mold_cavity = mk_mold_cavity()
mold_core = mk_mold_core()
mold_core_base = mk_mold_core_base()

# Optionally, fit the view to see the whole object using FreeCAD GUI
Gui.ActiveDocument.ActiveView.viewAxometric()
Gui.SendMsgToActiveView("ViewFit")

# Save the document
doc.saveAs(u"flatball-usau-mold.FCStd")
print(f"Document saved")
