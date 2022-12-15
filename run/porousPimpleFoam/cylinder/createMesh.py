from OpenFoamDataWriter import OpenFoamDataWriter
import numpy as np
import os

class pyMesh(object):
    def __init__(self,*args):       
        self._r = 0.1  #Cylinder diameter
        self._h = 0.5 #Cylinder height
        self._reduc = 0.65 #Meshing parameter controller the inner circle
        self._reduc2 = 0.53 #Inner circle (set to self._reduc for perfect circle)
        self._grading = 5 #Grading towards walls
        self._cellDensity = 10 #Cell density in mesh
        
    def _create_one_level_data(self, r, theta, z):
        return [
            (np.cos(theta)*r, np.sin(theta)*r, z),
        ]

    def _create_points_data(self):
        points_array = []

        for z in [0, self._h]:
            for r in [self._r, self._r*self._reduc]:
                for theta in [0, 1/2*np.pi, np.pi, 3/2*np.pi]:
                    points_array = points_array + self._create_one_level_data(r, theta, z)
        return points_array

    def _create_one_level_edge_data(self, layer, z):
        nVerticesPerLayer = 8
        return [
            "arc %i %i (%e %e %e) " % (0+layer*nVerticesPerLayer, 1+layer*nVerticesPerLayer, np.cos(1/4*np.pi) * self._r, np.sin(1/4*np.pi) * self._r, z),
            "arc %i %i (%e %e %e) " % (1+layer*nVerticesPerLayer, 2+layer*nVerticesPerLayer, np.cos(3/4*np.pi) * self._r, np.sin(3/4*np.pi) * self._r, z),
            "arc %i %i (%e %e %e) " % (2+layer*nVerticesPerLayer, 3+layer*nVerticesPerLayer, np.cos(5/4*np.pi) * self._r, np.sin(5/4*np.pi) * self._r, z),
            "arc %i %i (%e %e %e) " % (3+layer*nVerticesPerLayer, 0+layer*nVerticesPerLayer, np.cos(7/4*np.pi) * self._r, np.sin(7/4*np.pi) * self._r, z),
            "arc %i %i (%e %e %e) " % (4+layer*nVerticesPerLayer, 5+layer*nVerticesPerLayer, np.cos(1/4*np.pi) * self._r*self._reduc2, np.sin(1/4*np.pi) * self._r*self._reduc2, z),
            "arc %i %i (%e %e %e) " % (5+layer*nVerticesPerLayer, 6+layer*nVerticesPerLayer, np.cos(3/4*np.pi) * self._r*self._reduc2, np.sin(3/4*np.pi) * self._r*self._reduc2, z),
            "arc %i %i (%e %e %e) " % (6+layer*nVerticesPerLayer, 7+layer*nVerticesPerLayer, np.cos(5/4*np.pi) * self._r*self._reduc2, np.sin(5/4*np.pi) * self._r*self._reduc2, z),
            "arc %i %i (%e %e %e) " % (7+layer*nVerticesPerLayer, 4+layer*nVerticesPerLayer, np.cos(7/4*np.pi) * self._r*self._reduc2, np.sin(7/4*np.pi) * self._r*self._reduc2, z),
        ]

    def _create_edges_data(self):
        edges_array = []
        
        layer = 0
        for z in [0, self._h]:
            edges_array = edges_array + self._create_one_level_edge_data(layer, z)
            layer = layer+1
        return edges_array

    def _create_one_level_block_data(self):

        return [
            "hex (0 1 5 4 8 9 13 12) porosity (%i %i %i) " % (self._cellDensity, self._cellDensity, 0.5*self._h/(self._reduc*self._r)*self._cellDensity) + " edgeGrading (%f %f %f %f %f %f %f %f %f %f %f %f)" % (
                1, 1, 1, 1, self._grading, self._grading, self._grading, self._grading, 1, 1, 1, 1),
            "hex (1 2 6 5 9 10 14 13) porosity (%i %i %i) " % (self._cellDensity, self._cellDensity, 0.5*self._h/(self._reduc*self._r)*self._cellDensity) + " edgeGrading (%f %f %f %f %f %f %f %f %f %f %f %f)" % (
                1, 1, 1, 1, self._grading, self._grading, self._grading, self._grading, 1, 1, 1, 1),
            "hex (2 3 7 6 10 11 15 14) porosity (%i %i %i) " % (self._cellDensity, self._cellDensity, 0.5*self._h/(self._reduc*self._r)*self._cellDensity) + " edgeGrading (%f %f %f %f %f %f %f %f %f %f %f %f)" % (
                1, 1, 1, 1, self._grading, self._grading, self._grading, self._grading, 1, 1, 1, 1),
            "hex (3 0 4 7 11 8 12 15) porosity (%i %i %i) " % (self._cellDensity, self._cellDensity, 0.5*self._h/(self._reduc*self._r)*self._cellDensity) + " edgeGrading (%f %f %f %f %f %f %f %f %f %f %f %f)" % (
                1, 1, 1, 1, self._grading, self._grading, self._grading, self._grading, 1, 1, 1, 1),
            "hex (4 5 6 7 12 13 14 15) porosity (%i %i %i) " % (self._cellDensity, self._cellDensity, 0.5*self._h/(self._reduc*self._r)*self._cellDensity) + " edgeGrading (%f %f %f %f %f %f %f %f %f %f %f %f)" % (
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)
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
            "       (1 0 4 5)",
            "       (2 1 5 6)",
            "       (3 2 6 7)",
            "       (0 3 7 4)",
            "       (5 4 7 6)",
            "       );",
            "   }",
            "   walls",
            "   {",
            "      type wall;",
            "      faces",
            "      (",
            "      (0 1 9 8)",
            "      (1 2 10 9)",
            "      (2 3 11 10)",
            "      (3 0 8 11)",
            "      );",
            "   }",
            "   top",
            "   {",
            "      type patch;",
            "      faces",
            "      (",
            "      (8 9 13 12)",
            "      (9 10 14 13)",
            "      (10 11 15 14)",
            "      (11 8 12 15)",
            "      (12 13 14 15)",
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