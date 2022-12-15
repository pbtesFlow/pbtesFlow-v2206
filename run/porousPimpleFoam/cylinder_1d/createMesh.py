from OpenFoamDataWriter import OpenFoamDataWriter
import numpy as np
import os

class pyMesh(object):
    def __init__(self, *args):       
        self._r = 0.1 #Cylinder diameter
        self._h = 0.5 #Cylinder height
        self._cellDensity_z = 100 #Cell density in z-direction per meter
        self.A = np.pi*self._r**2
        self.dx = np.sqrt(self.A)
        
    def _create_one_level_data(self):
        return [
            (-self.dx/2, -self.dx/2, 0),
            (self.dx/2, -self.dx/2, 0),
            (self.dx/2, self.dx/2, 0),
            (-self.dx/2, self.dx/2, 0),
            (-self.dx/2, -self.dx/2, self._h),
            (self.dx/2, -self.dx/2, self._h),
            (self.dx/2, self.dx/2, self._h),
            (-self.dx/2, self.dx/2, self._h),
        ]

    def _create_points_data(self):
        points_array = []
        points_array = points_array + self._create_one_level_data()
        return points_array

    def _create_edges_data(self):
        edges_array = []
        return edges_array

    def _create_one_level_block_data(self):
        return [
            "hex (0 1 2 3 4 5 6 7) porosity (%i %i %i) " % (1, 1, self._h*self._cellDensity_z) + " simpleGrading (1 1 1)"
        ]

    def _create_block_data(self):
        blocks_array = []
        blocks_array = blocks_array + self._create_one_level_block_data()
        return blocks_array

    def _create_boundary_data(self):
        boundary_array = [
            "   bottom",
            "   {",
            "       type patch;",
            "       faces",
            "       (",
            "       (0 3 2 1)",
            "       );",
            "   }",
            "   walls",
            "   {",
            "      type wall;",
            "      faces",
            "      (",
            "      (4 7 3 0)",
            "      (3 7 6 2)",
            "      (1 2 6 5)",
            "      (0 1 5 4)",
            "      );",
            "   }",
            "   top",
            "   {",
            "      type patch;",
            "      faces",
            "      (",
            "      (7 4 5 6)",
            "      );",
            "   }",
        ]
        return boundary_array

    def write_block_mesh_dict(self, filename="blockMeshDict"):
        points_data = self._create_points_data()
        entries = []
        entries.append('scale   1;\n')
        entries.append('vertices')
        entries.append('(')
        for index, one_point in enumerate(points_data):
            entries.append("    (%e %e %e) " % one_point + "// %i" % index)
        entries.append(");\n")

        entries.append("blocks")
        entries.append("(")
        for each_entry in self._create_block_data():
            entries.append(each_entry)
        entries.append(");\n")

        entries.append("edges")
        entries.append("(")
        for each_entry in self._create_edges_data():
            entries.append(each_entry)
        entries.append(");\n")

        entries.append("boundary")
        entries.append("(")
        for each_entry in self._create_boundary_data():
            entries.append(each_entry)
        entries.append(");\n")

        entries.append("mergePatchPairs")
        entries.append("(")
        entries.append(");\n")

        dirName = os.getcwd() + ""
        writer = OpenFoamDataWriter(dirName, "system/", filename, entries)

if __name__ == '__main__':
    mesh = pyMesh()    
    mesh.write_block_mesh_dict()